<?php

/**
 * Payment method callback: payment transfer form.
 */
function commerce_oz_migs_3p_redirect_form($form, &$form_state, $order, $payment_method) {

	$logopath = '/' . drupal_get_path('module', 'commerce_oz') . '/image/' . $payment_method['gateway_provider'] . '_68.png';
	
	$descText = '<br /><a href="http://www.eway.com.au"><img src="' . $logopath . '" /></a><br /><br />Click the \'Proceed to eWayz\' button to complete payment of your purchase at eWay.<br />On completing your transaction, you will be taken to your Receipt information.<br />';

  $form['commerce_oz_3p_head'] = array(
    '#type' => 'fieldset',
    '#title' => $payment_method['title'],
    '#collapsible' => FALSE, 
  	'#collapsed' => FALSE,
    '#description' => t($descText),
  );

	// Transaction record
  $transaction =  _commerce_oz_open_transaction($payment_method['method_id'], $order->order_id);

	$wrapper = entity_metadata_wrapper('commerce_order', $order);

  // Get financial info
  $currency_code = $wrapper->commerce_order_total->currency_code->value();
  $amount = $wrapper->commerce_order_total->amount->value();
	
	$request_map = _commerce_oz_migs_build_3p_purchase_request($payment_method, $order, $transaction, $amount);
	
	$responseurl = $request_map['vpc_ReturnURL'];

	
  // Save the transaction with the request_map.
	$transaction->payload = array('request' => _process_vpc_request_map($request_map));
 	commerce_payment_transaction_save($transaction);

	$request_string = http_build_query($request_map);
	
	$poststring = 'https://migs.mastercard.com.au/vpcpay?' . $request_string;

	dsm($poststring, '$poststring');

//	$form['#action'] = $poststring;
	
//	$form_state['redirect'] = $poststring;
	
  $form['commerce_oz_3p_head']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Proceed to Commonwealth Bank'),
  );
	
	drupal_goto($poststring);
	
	return $form;
	
}



function commerce_oz_migs_3p_redirect_form_validate($order, $payment_method)
{

  dsm($_GET, '$_GET');
  
  $trans_ref_array = explode('-', $_GET['vpc_MerchTxnRef']);
  
  $transaction = commerce_payment_transaction_load($trans_ref_array[1]);
  
  $settings = _commweb_get_var('commerce_oz_gateway_' . $payment_method['gateway_provider']);
  
  dsm($transaction, '$transaction returned');
  
  $request_map = $transaction->payload['request'];
  $response_map = $_GET;
  
 	_commerce_oz_migs_3p_close_transaction($transaction, $request_map, $response_map);
	
	_commerce_oz_debug_transaction($transaction, $request_map, $response_map, $settings['commerce_oz_debug_mode']);
	
	// Fire completion Rule with the refund transaction
	rules_invoke_event('commerce_oz_transaction_complete', $transaction);
	
	return _commerce_oz_handle_payment_response($transaction, $request_map, $response_map); 
   
   
}

/**
 * Payment method callback: payment transfer form.
 * Could be shown on Review Screen?
 */
function commerce_oz_migs_3p_payment_submit_form($payment_method, $pane_values, $checkout_pane, $order) {

  $form = array();
	
	$gateway_provider = commerce_oz_load_gateway_provider($payment_method['gateway_provider']);
	
	$logopath = '/' . drupal_get_path('module', 'commerce_commweb') . '/image/' . $gateway_provider['base']  . '_68.png';
	
	$descText = '<img src="' . $logopath . '" /><br />Click the Pay Now button to be transferred to the ' .  $gateway_provider['owner'] . ' Payment Servers.<br />On completing your transaction, you will be returned to this page for your Receipt information.<br />';

  $form['commerce_oz_payment_transfer'] = array(
    '#type' => 'fieldset',
    '#title' => t($payment_method['title']),
    '#collapsible' => FALSE, 
  	'#collapsed' => FALSE,
    '#description' => t($descText),
  );
	
	
  $form['commerce_oz_payment_transfer']['pay_now_button'] = array(
    '#type' => 'button',
    '#prefix' => '<br />',
    '#value' => t('Pay Now'), 
   
  );
	
  return $form;
}




function _process_vpc_request_map($request_map)
{
		$request_map['vpc_Merchant'] = _commweb_build_merchant_string($request_map['vpc_Merchant']);
		$request_map['vpc_AccessCode'] = _commweb_build_accesscode_string($request_map['vpc_AccessCode']);
		return $request_map;
}


function _commerce_oz_migs_build_3p_purchase_request($payment_method, $order, $transaction, $amount)
{
		// Get the API Credentials
		
		$settings = _commweb_get_var('commerce_oz_gateway_' . $payment_method['gateway_provider']);
		
		$merchantid = $settings['merchant_id'];
		$accesscode = $settings['access_code'];
		$secret_hash = $settings['secure_secret_hash'];
		
		//Generate the MerchantTxnRef
		$merchTxnRef = $order->order_id . '-' . $transaction->transaction_id;
		
		// Assemble the array
		$request_map = array(
			'vpc_AccessCode' => $accesscode,
			'vpc_Amount' => $amount,
			'vpc_Command' => 'pay',
			'vpc_Locale' => 'en',
			'vpc_MerchTxnRef' => $merchTxnRef,
			'vpc_Merchant' => $merchantid, 
			'vpc_OrderInfo' => $order->order_id,
			'vpc_ReturnURL' => url('checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key'], 
															array('absolute' => TRUE)
														),
			'vpc_Version' => '1',
		);
			
		$md5HashData = $secret_hash;
			
		foreach($request_map as $key => $value) {
			$md5HashData .= $value;			
		}
		
    $request_map['vpc_SecureHash'] = strtoupper(md5($md5HashData));

		return $request_map;	
}


function _commerce_oz_migs_3p_close_transaction($transaction,  $request_map, $response_map) {
	
	// Cleanse the payload arrays of any undesirable data
	$request_map['vpc_Merchant'] = _commweb_build_merchant_string($request_map['vpc_Merchant']);
	$request_map['vpc_AccessCode'] = _commweb_build_accesscode_string($request_map['vpc_AccessCode']);
	$response_map['vpc_Merchant'] = _commweb_build_merchant_string($response_map['vpc_Merchant']);
		
	$transaction->instance_id = 	$transaction->payment_method . '|commerce_payment_' . $transaction->payment_method;
 	$transaction->amount = 				$response_map['vpc_Amount'];
  $transaction->currency_code = $transaction->currency_code;
  
  $transaction->payload = array('request' => $transaction->payload['request'], 
  															'response' => $response_map,
  															);
  
	$transaction->remote_id = 		$response_map['vpc_TransactionNo'];    
  $transaction->remote_status = $response_map['vpc_TxnResponseCode'];  
  
  $transaction->status = 				$response_map['vpc_TxnResponseCode'] ? COMMERCE_PAYMENT_STATUS_FAILURE : COMMERCE_PAYMENT_STATUS_SUCCESS;
  
  $transaction->message = 			t('<strong>Purchase</strong> Result: @code - @message');

  $transaction->message_variables = array(
  	'@code' => $transaction->status,
  	'@message' => $response_map['vpc_Message'],
  		);
  		
  // Save the refund transaction information.
  commerce_payment_transaction_save($transaction);		
 
  return $response_map;			
}





