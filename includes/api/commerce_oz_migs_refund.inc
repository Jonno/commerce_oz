<?php

/**
 * @file
 * Implements Commerce Oz refunds for use in Drupal Commerce.
 */


function commerce_oz_migs_refund($order, $original_transaction, $gateway) {

//	dsm($order, '$order for refund');
//	dsm($original_transaction, '$transaction for refund');
  	 	 	
  // Get the order
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $order_id = $order->order_id;
  
	$settings = _commweb_get_var('commerce_oz_gateway_' . $gateway['base']);
 	
 
	// Get a Refund Transaction record
  $refund_transaction = _commerce_oz_open_transaction($original_transaction->payment_method, $order->order_id);
  
  $amount = $order->commerce_order_total['und'][0]['amount'];  
  
	$request_map = commerce_oz_migs_build_refund_request($gateway, $original_transaction, $refund_transaction, $order, $amount, $settings);

	$response_map = commerce_oz_migs_post_refund_request($request_map);

// 	dsm($request_map, '$request_map');
 //	dsm($response_map, '$response_map');

   	
  _commerce_oz_migs_refund_close_transaction($refund_transaction, $original_transaction, $request_map, $response_map);
	
	_commerce_oz_debug_transaction($refund_transaction, $request_map, $response_map, $settings['commerce_oz_debug_mode']);
	
	// Fire completion Rule with the refund transaction
	rules_invoke_event('commerce_oz_transaction_complete', $refund_transaction);
	
	return _commerce_oz_handle_payment_response($refund_transaction, $request_map, $response_map);


}



function commerce_oz_migs_build_refund_request($gateway, $original_transaction, $refund_transaction, $order, $amount, $settings) {
		
		// Get the API Credentials
		
		$merchantid = $settings['merchant_id'];
		$accesscode = $settings['access_code'];
		$user = $settings['refund_username'];
		$password = $settings['refund_password'];
		
		$postData = '';
		$amp = '&';
		
		// Merchant Reference for refund
		// format : order_id-original_trans_id-new_trans_id
		
		$merchant_reference = $original_transaction->payload['request']['vpc_MerchTxnRef'] . '-' . $refund_transaction->transaction_id;
		
		$request_map = array(
		
			'vpc_Version' => COMMERCE_COMMWEB_vpc_Version,
			'vpc_Command' => 'refund',
			'vpc_MerchTxnRef' => $merchant_reference,
			'vpc_AccessCode' => $accesscode,
			'vpc_Merchant' => $merchantid,
			'vpc_TransNo' => $original_transaction->payload['response']['vpc_TransactionNo'],
			'vpc_Amount' => $amount,
			'vpc_User' => $user,
			'vpc_Password' => $password,
		);
				
		return $request_map;
}


function commerce_oz_migs_post_refund_request($request_map) {

	$request_string = http_build_query($request_map);

	ob_start();
	
	// initialise
	$curlhandle = curl_init();
	
	// set the URL of the VPC
	curl_setopt ($curlhandle, CURLOPT_URL, COMMERCE_COMMWEB_URL);
	curl_setopt ($curlhandle, CURLOPT_POST, 1);
	curl_setopt ($curlhandle, CURLOPT_POSTFIELDS, $request_string);
	curl_setopt ($curlhandle, CURLOPT_SSL_VERIFYPEER, 0);
	curl_setopt ($curlhandle, CURLOPT_SSL_VERIFYHOST, 0);
	
	// connect
	curl_exec ($curlhandle);
	
	// get response
	$response_string = ob_get_contents();
	
	// turn output buffering off.
	ob_end_clean();
	
	$response_map = array();
	parse_str($response_string, $response_map);
	
	return $response_map;

}


function _commerce_oz_migs_refund_close_transaction($transaction, $original_transaction, $request_map, $response_map) {

	// Update the transaction object
	
	// Cleanse the payload arrays of any undesirable data
		$request_map['vpc_Merchant'] = _commweb_build_merchant_string($request_map['vpc_Merchant']);
		$request_map['vpc_AccessCode'] = _commweb_build_accesscode_string($request_map['vpc_AccessCode']);
		$request_map['vpc_Password'] = _commweb_build_refund_user_password_string($request_map['vpc_Password']);
		$request_map['vpc_User'] = _commweb_build_refund_user_string($request_map['vpc_User']);
	
	$refunded = array_filter((array)$original_transaction);
	
	$transaction->instance_id = $original_transaction->instance_id;
 	$transaction->amount = - $response_map['vpc_Amount'];
  $transaction->currency_code = $original_transaction->currency_code;
  
  $transaction->payload = array('refunded' => $refunded,
  															'request' => $request_map, 
  															'response' => $response_map
  															);
  
	$transaction->remote_id = $response_map['vpc_TransactionNo'];    
  $transaction->remote_status = $response_map['vpc_TxnResponseCode'];  
  
  $transaction->status = $response_map['vpc_TxnResponseCode'] ? COMMERCE_PAYMENT_STATUS_FAILURE : COMMERCE_PAYMENT_STATUS_SUCCESS;
  
  $transaction->message = '<strong>Refund</strong> Code: @code - @message<br />Order ID: <a href="/admin/commerce/orders/@order_id">@order_id</a><br />Original Transaction: <a href="/admin/commerce/orders/@order_id/payment/@trans_id/view">@trans_id</a>'; 

  $transaction->message_variables = array(
  	'@code' => $transaction->status,
  	'@message' => $response_map['vpc_Message'],
  	'@trans_id' => $original_transaction->transaction_id,
  	'@order_id' => $original_transaction->order_id,
  		);
  		
  // Save the refund transaction information.
  commerce_payment_transaction_save($transaction);		
  
  // Update the original transaction
  $original_transaction->message = '<strong>Purchase</strong> Code: @code - @message<br />'; 
  $original_transaction->message .= '<strong>Refunded</strong> Code: @refcode - @refmessage<br />';
  $original_transaction->message .= 'Refund Transaction: <a href="/admin/commerce/orders/@reforder_id/payment/@reftrans_id/view">@reftrans_id</a>';

  $original_transaction->message_variables = array(
  			'@code' => $original_transaction->message_variables['@code'],
  			'@message' => $original_transaction->message_variables['@message'],
  			'@refcode' => $transaction->message_variables['@code'],
  			'@refmessage' => $transaction->message_variables['@message'],
  			'@reforder_id' => $original_transaction->order_id,
  			'@reftrans_id' => $transaction->transaction_id,
  		);
 
  // Save the original transaction information.
  commerce_payment_transaction_save($original_transaction);		
 
  
  return $response_map;			
}















function commerce_oz_refund_process_migs_refund_request($request_string)
{
		
		module_load_include('inc', 'commerce_oz', 'commerce_oz_migs_2p');
	  $request_map = array();
		parse_str($request_string, $request_map);
		$request_map['vpc_Merchant'] = _commweb_build_merchant_string($request_map['vpc_Merchant']);
		$request_map['vpc_AccessCode'] = _commweb_build_accesscode_string($request_map['vpc_AccessCode']);
		$request_map['vpc_Password'] = _commweb_build_refund_user_password_string($request_map['vpc_Password']);
		$request_map['vpc_User'] = _commweb_build_refund_user_string($request_map['vpc_User']);
		return $request_map;
}


function commerce_oz_refund_process_vpc_refund_response($transaction, $response_string, $request_map, $order_id = 0, $old_trans_id = 0, $test_message = '')
{
	// Map VPC repsonse to array
	$response_map = array();
	parse_str($response_string, $response_map);
	$response_map['vpc_Merchant'] = _commweb_build_merchant_string($response_map['vpc_Merchant']);
	
	// Update the transaction object
  $transaction->instance_id = 'commerce_commweb|commerce_payment_commerce_commweb';
  $transaction->amount = $response_map['vpc_Amount'];
  $transaction->currency_code = 'AUD';
  $transaction->payload = array('request' => $request_map, 
  															'response' => $response_map
  															);
  
  $transaction->remote_id = $response_map['vpc_TransactionNo'];    
  $transaction->remote_status = $response_map['vpc_TxnResponseCode'];  
  
  $transaction->status = $response_map['vpc_TxnResponseCode'] ? COMMERCE_PAYMENT_STATUS_FAILURE : COMMERCE_PAYMENT_STATUS_SUCCESS;
  
  $transaction->message = $test_message . '<strong>Refund</strong> Code: @code - @message <br />Order ID: <a href="/admin/commerce/orders/@order_id">@order_id</a> <br />Refunded Transaction: <a href="/admin/commerce/orders/@order_id/payment/@trans_id/view">@trans_id</a>'; 
  
  $transaction->message_variables = array(
  	'@code' => $response_map['vpc_TxnResponseCode'],
  	'@message' => $response_map['vpc_Message'],
  	'@trans_id' => $old_trans_id,
  	'@order_id' => $order_id,
  		);
  
  return $response_map;		
  		
}





