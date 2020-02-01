<?php

/**
 * @file
 * Implements PaymentExpress refunds for use in Drupal Commerce.
 */



/**
 * Payment method refund callback:
 *
 * This is the function that handles the refund calls to servers
 */
function commerce_oz_paymentexpress_refund($order, $original_transaction, $gateway) {
 
 	dsm($order, '$order for refund');
	dsm($original_transaction, '$transaction for refund');
  	 	 	
  // Get the order
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $order_id = $order->order_id;
  
	$settings = _commweb_get_var('commerce_oz_gateway_paymentexpress');
 
	// Transaction record
	// Prepare a refund transaction to log the API response.
  $refund_transaction = _commerce_oz_open_transaction($original_transaction->payment_method, $order->order_id);
  
  $amount = $order->commerce_order_total['und'][0]['amount'] / 100;  // PXPost uses decimal values not cent values
  
	$request_map = _commerce_oz_pxpost_build_refund_request($original_transaction, $refund_transaction, $order, $amount, $settings);

	$response_map = _commerce_oz_pxpost_post_refund_request($request_map);

 	dsm($request_map, '$request_map');
 	dsm($response_map, '$response_map');
  
  _commerce_oz_paymentexpress_refund_close_transaction($refund_transaction, $original_transaction, $request_map, $response_map);
	
	_commerce_oz_debug_transaction($refund_transaction, $request_map, $response_map, $settings['commerce_oz_debug_mode']);
	
	// Fire completion Rule with the refund transaction
	rules_invoke_event('commerce_oz_transaction_complete', $refund_transaction);
	
	return _commerce_oz_handle_payment_response($refund_transaction, $request_map, $response_map);
 
}




/**
 * Payment method refund callback:
 *
 * Builds the Request Map array for the call to to server and logging
 */
function _commerce_oz_pxpost_build_refund_request($transaction, $refund_transaction, $order, $amount, $settings) {
		
		$merchant_reference = $transaction->payload['request']['MerchantReference'] . '-' . $refund_transaction->transaction_id;
		
		$request_map = array(
			'PostUsername' => 	$settings['pxpost_username'],
			'PostPassword' => 	$settings['pxpost_password'],
			'Amount' => 				$amount, //$order->commerce_order_total['und'][0]['amount'],
			'DpsTxnRef' =>			$transaction->remote_id,
			'InputCurrency' => 	$order->commerce_order_total['und'][0]['currency_code'],
			'TxnType' => 				'Refund',
			'MerchantReference' => $merchant_reference,
		);
		
		return $request_map;
}

function _commerce_oz_pxpost_post_refund_request($request_map) {
		
		$request_xml = _commerce_oz_array2xml($request_map, '<Txn/>');
		$request_xml = str_replace('<?xml version="1.0"?>', '', $request_xml);
				
		$URL = "https://sec.paymentexpress.com/pxpost.aspx";
		              
		$ch = curl_init(); 
		curl_setopt($ch, CURLOPT_URL,  $URL);
		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_POSTFIELDS, $request_xml);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0); //Needs to be included if no *.crt is available to verify SSL certificates
		curl_setopt($ch, CURLOPT_SSLVERSION,3);  
		$result = curl_exec ($ch); 
		curl_close ($ch);
		
		return json_decode(json_encode((array) simplexml_load_string($result)), 1);
}



function _commerce_oz_paymentexpress_refund_close_transaction($transaction, $original_transaction, $request_map, $response_map) {

	// Update the transaction object
	
	// Cleanse the payload arrays of any undesirable data
	$request_map['PostUsername'] = _commweb_build_merchant_string($request_map['PostUsername']);
	$request_map['PostPassword'] = _commweb_build_accesscode_string($request_map['PostPassword']);
	
	$refunded = array_filter((array)$original_transaction);
	
	$transaction->instance_id = $original_transaction->instance_id;
 	$transaction->amount = - $response_map['Transaction']['Amount'] * 100; // PXPost uses decimal values not cents
  $transaction->currency_code = $response_map['Transaction']['InputCurrencyName'];
  
  
  $transaction->payload = array('refunded' => $refunded,
  															'request' => $request_map, 
  															'response' => $response_map
  															);
  
  $transaction->remote_id = $response_map['DpsTxnRef'];    
  $transaction->remote_status = $response_map['Success'];  
  
  $transaction->status = $transaction->remote_status ? COMMERCE_PAYMENT_STATUS_SUCCESS : COMMERCE_PAYMENT_STATUS_FAILURE;
  
  $transaction->message = '<strong>Refund</strong> Code: @code - @message<br />Order ID: <a href="/admin/commerce/orders/@order_id">@order_id</a><br />Original Transaction: <a href="/admin/commerce/orders/@order_id/payment/@trans_id/view">@trans_id</a>'; 

  $transaction->message_variables = array(
  	'@code' => $transaction->status,
  	'@message' => $response_map['ResponseText'],
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





