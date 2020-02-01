<?php

/**
 * @file
 * Implements Commerce Oz refunds for use in Drupal Commerce.
 */


function commerce_oz_ematters_refund($order, $original_transaction, $gateway) {

//	dsm($order, '$order for refund');
//	dsm($original_transaction, '$transaction for refund');
  	 	 	
  // Get the order
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $order_id = $order->order_id;
  
	$settings = _commweb_get_var('commerce_oz_gateway_' . $gateway['base']);
 	
 
	// Get a Refund Transaction record
  $refund_transaction = _commerce_oz_open_transaction($original_transaction->payment_method, $order->order_id);
  
  $amount = $order->commerce_order_total['und'][0]['amount'] / 100;  // $1.23 value
  
	$request_map = commerce_oz_ematters_build_refund_request($gateway, $original_transaction, $refund_transaction, $order, $amount, $settings);

	$response_map = commerce_oz_ematters_post_refund_request($request_map);

 	dsm($request_map, '$request_map');
 	dsm($response_map, '$response_map');

   	
  _commerce_oz_ematters_refund_close_transaction($refund_transaction, $original_transaction, $request_map, $response_map);
	
	_commerce_oz_debug_transaction($refund_transaction, $request_map, $response_map, $settings['commerce_oz_debug_mode']);
	
	// Fire completion Rule with the refund transaction
	rules_invoke_event('commerce_oz_transaction_complete', $refund_transaction);
	
	return _commerce_oz_handle_payment_response($refund_transaction, $request_map, $response_map);


}



function commerce_oz_ematters_build_refund_request($gateway, $original_transaction, $refund_transaction, $order, $amount, $settings) {
	
		// Merchant Reference for refund
		// format : order_id-original_trans_id-new_trans_id
		
		$original_transaction_ref = $original_transaction->payload['request']['UID'];
		$original_payment_amount = $original_transaction->payload['response']['emattersAmount'];
		
		$merchant_reference =  $original_transaction_ref . '-' . $refund_transaction->transaction_id;

		// Make the request_map array
		$request_map = array(
			'readers' 								=> $settings['ematters_customer_id'],
			'pin' 										=> $settings['ematters_refund_pin'],
			'password' 								=> $settings['ematters_poster_password'],
			'UID' 										=> $merchant_reference,
			'Trxn' 										=> $original_transaction_ref,	
			'RefundAmount' 						=> $original_payment_amount, 
			'Action' 									=> 'RefundTrans',
		);
		
		return $request_map;

}


function commerce_oz_ematters_post_refund_request($request_map) {
		
		$request_xml = _commerce_oz_array2xml($request_map, "<?xml version='1.0' encoding='ISO-8859-1'?><ematters />");
		
		$request_xml = str_replace(' ', '', $request_xml);
		
		dsm($request_xml, '$request_xml');

		$URL = "https://merchant.ematters.com.au/cmaonline.nsf/RefundTransaction?OpenAgent";
		              
		$ch = curl_init(); 
		curl_setopt($ch, CURLOPT_URL,  $URL);
		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_POSTFIELDS, $request_xml);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0); //Needs to be included if no *.crt is available to verify SSL certificates
		curl_setopt($ch,CURLOPT_SSLVERSION,3);  
		$result = curl_exec ($ch); 
		curl_close ($ch);
		
		dsm($result, '$result');
		
		return json_decode(json_encode((array) simplexml_load_string($result)),1);;
}


function _commerce_oz_ematters_refund_close_transaction($transaction, $original_transaction, $request_map, $response_map) {

	// Update the transaction object
	
	// Cleanse the payload arrays of any undesirable data
	$request_map['readers'] 		= _commweb_build_merchant_string($request_map['readers']);
	$request_map['password'] 		= _commweb_build_accesscode_string($request_map['password']);
	$request_map['pin']					= _commweb_build_refund_user_password_string($request_map['pin']);
	
	$refunded = array_filter((array)$original_transaction);
	
	$transaction->instance_id 	= $original_transaction->instance_id;
 	$transaction->amount 				= - $response_map['emattersAmount'];
  $transaction->currency_code = $original_transaction->currency_code;
  
  $transaction->payload 			= array('refunded' => $refunded,
			  															'request'  => $request_map, 
			  															'response' => $response_map
			  															);
  
  $transaction->remote_id 		= $response_map['emattersMainID'];    
  $transaction->remote_status = $response_map['emattersRcode']; 
  
  $transaction->status 				= ($transaction->remote_status === '08') 
  																? COMMERCE_PAYMENT_STATUS_SUCCESS 
  																: COMMERCE_PAYMENT_STATUS_FAILURE;
  																
  // Save the refund transaction information.
  commerce_payment_transaction_save($transaction);
	
  
	commerce_oz_update_refund_transaction_messages($response_map['ResponseMessage'], 
																									$refund_transaction, 
																									$original_transaction);	
 
  
  return $response_map;			
}




