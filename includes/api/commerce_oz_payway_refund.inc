<?php

/**
 * @file
 * Implements Commerce Oz refunds for use in Drupal Commerce.
 */


function commerce_oz_payway_refund($order, $original_transaction, $gateway) {

//	dsm($order, '$order for refund');
//	dsm($original_transaction, '$transaction for refund');
  	 	 	
  // Get the order
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $order_id = $order->order_id;
  
	$settings = _commweb_get_var('commerce_oz_gateway_' . $gateway['base']);
 	
 
	// Get a Refund Transaction record
  $refund_transaction = _commerce_oz_open_transaction($original_transaction->payment_method, $order->order_id);
  
  $amount = $order->commerce_order_total['und'][0]['amount'];  
  
	$request_map = commerce_oz_payway_build_refund_request($gateway, $original_transaction, $refund_transaction, $order, $amount, $settings);

	$response_map = commerce_oz_payway_post_refund_request($request_map);

   	
  _commerce_oz_payway_refund_close_transaction($refund_transaction, $original_transaction, $request_map, $response_map);
	
	_commerce_oz_debug_transaction($refund_transaction, $request_map, $response_map, $settings['commerce_oz_debug_mode']);
	
	// Fire completion Rule with the refund transaction
	rules_invoke_event('commerce_oz_transaction_complete', $refund_transaction);
	
	return _commerce_oz_handle_payment_response($refund_transaction, $request_map, $response_map);


}



function commerce_oz_payway_build_refund_request($gateway, $original_transaction, $refund_transaction, $order, $amount, $settings) {
		
		global $user;
		
		// Get the API Credentials
		
  	$settings = _commweb_get_var('commerce_oz_gateway_payway');	
		
	
		$request_map = array();
    $request_map[ "order.type" ] = "refund";
    $request_map[ "customer.username" ] = $settings['payway_username'];
    $request_map[ "customer.password" ] = $settings['payway_password'];
    $request_map[ "customer.merchant" ] = $settings['payway_merchant'];
    
    $request_map[ "customer.orderNumber" ] = $original_transaction->payload['request']['customer.orderNumber'] . '-' . $refund_transaction->transaction_id;
    
    $request_map[ "customer.customerReferenceNumber" ] = $user->uid;    
  	$request_map[ "customer.originalOrderNumber" ] = $original_transaction->payload['request']['customer.orderNumber'];  
    $request_map[ "order.amount" ] = $amount;
    $request_map[ "order.ECI" ] = 'SSL';
					
		return $request_map;
}


function commerce_oz_payway_post_refund_request($request_map) {


	$path = drupal_get_path('module', 'commerce_oz');
	require_once($path . '/includes/api/Qvalent_PayWayAPI.php');

 /// watchdog('payway', nl2br(print_r($pane_values, TRUE)));
  
	// Get the payment method settings
  $settings = _commweb_get_var('commerce_oz_gateway_payway');	
  $debugmode = $settings['commerce_oz_debug_mode'];
  $livemode = $settings['commerce_oz_transaction_mode'];
 
  $path_to_cert = $path . '/includes/api/cacerts.crt';
  $initParams =
        "certificateFile=" . $settings['certificate_path'] . '&' .
        "caFile=" . $path_to_cert . '&' .
        "logDirectory=" . 'sites/commerce.localhost/files';
        
  $paywayAPI = new Qvalent_PayWayAPI();
  $paywayAPI->initialise( $initParams );
    
  $requestText = $paywayAPI->formatRequestParameters( $request_map );
  	
 	watchdog('payway', nl2br(print_r($request_map, TRUE)));
  	
  $responseText = $paywayAPI->processCreditCard( $requestText );

    // Parse the response string into an array
  $response_map = $paywayAPI->parseResponseParameters( $responseText );

 // 	dsm($responseParameters);
  watchdog('payway', nl2br(print_r($response_map, TRUE)));  
	
	return $response_map;

}


function _commerce_oz_payway_refund_close_transaction($transaction, $original_transaction, $request_map, $response_map) {

	// Update the transaction object
	
	// Cleanse the payload arrays of any undesirable data
	$request_map['customer.username'] = _commweb_build_merchant_string($request_map['customer.username']);
	$request_map['customer.password'] = _commweb_build_accesscode_string($request_map['customer.password']);
	
	$refunded = array_filter((array)$original_transaction);
	
	$transaction->instance_id = $original_transaction->instance_id;
 	$transaction->amount = - $request_map['order.amount'];
  $transaction->currency_code = $original_transaction->currency_code;
  
  $transaction->payload = array('refunded' => $refunded,
  															'request' => $request_map, 
  															'response' => $response_map,
  															);
  
	$transaction->remote_id = $response_map['response.receiptNo'];    
  $transaction->remote_status = $response_map['response.summaryCode'];  // 0, 1, 2, 3
  $transaction->status = $transaction->remote_status ? COMMERCE_PAYMENT_STATUS_FAILURE : COMMERCE_PAYMENT_STATUS_SUCCESS;
  

  
  $transaction->message = '<strong>Refund</strong> Code: @code - @message<br />Order ID: <a href="/admin/commerce/orders/@order_id">@order_id</a><br />Original Transaction: <a href="/admin/commerce/orders/@order_id/payment/@trans_id/view">@trans_id</a>'; 

  $transaction->message_variables = array(
  	'@code' => $transaction->status,
  	'@message' => $response_map['response.text'],
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


