<?php

/**
 * @file
 * Implements PaymentExpress PXPost for use in Drupal Commerce.
 */

define('COMMERCE_OZ_PXPOST_2P_LIVE_URL', 'https://www.paymentexpress.com.au/gatpaymentexpress_cvn/xmlpayment.asp');
define('COMMERCE_OZ_PXPOST_2P_TEST_URL', 'https://www.paymentexpress.com.au/gatpaymentexpress_cvn/xmltest/testpage.asp');

/**
 * Payment method callback: settings form.
 */
function commerce_oz_paymentexpress_2p_settings_form($settings = NULL) {

  $form = array();
 
	$logopath = '/' . drupal_get_path('module', 'commerce_oz') . '/image/paymentexpress_68.png';
	
	$descText = '<a href="http://www.paymentexpress.com"><img src="' . $logopath . '" align ="left" /></a>Payment Method for the <a href="http://www.paymentexpress.com">Payment Express PXPost</a> (2 party) provided by <a href="http://www.paymentexpress.com">Payment Express</a> Payment Service.<br />No settings are provided here.';

  $form['intro_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('PaymentExpress PXPost'),
    '#collapsible' => FALSE, 
  	'#collapsed' => FALSE,
    '#description' => t($descText),
  );

  return $form;
}

/**
 * Payment method callback: checkout form.
 * loading the default form from Commerce Payment module
 */
function commerce_oz_paymentexpress_2p_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  	 	 	
  // Include the Drupal Commerce credit card file for forms
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');
  
  $form_params = array();
  $form_defaults = array();
  
  $settings = _commweb_get_var('commerce_oz_gateway_' . $payment_method['gateway_provider']);
  
  $livemode = $settings['commerce_oz_transaction_mode'];
	
	$form_params['owner'] = 'Card Holder';	
 	$form_params['code'] = 'CVV';
 

	$logopath = '/' . drupal_get_path('module', 'commerce_oz') . '/image/paymentexpress_68.png';
	
	$descText = '<a href="http://www.paymentexpress.com"><img src="' . $logopath . '" /></a><br />Enter your payment details below and click Continue to next step.<br />You will be taken to your Receipt information.<br />';

 // set the defaults for testing
 if($settings['commerce_oz_transaction_mode'] == 'test') {

	 	$form_defaults['number'] = $settings['commerce_oz_test_card_number'];
	 	$form_defaults['code'] = $settings['commerce_oz_test_card_cvv'];
	 	$form_defaults['exp_month'] = $settings['commerce_oz_test_card_exp_month'];
	 	$form_defaults['exp_year'] = $settings['commerce_oz_test_card_exp_year'];
	}

 	// Load the DC credit card form
 	$form = commerce_payment_credit_card_form($form_params, $form_defaults);

  $form['commerce_oz_2p_head'] = array(
    '#type' => 'fieldset',
    '#title' => $payment_method['title'],
    '#collapsible' => FALSE, 
  	'#collapsed' => FALSE,
    '#description' => t($descText),
  );

 if($settings['commerce_oz_transaction_mode'] == 'test')
 {
 
    $form['credit_card']['number']['#description'] = t('<strong>Test Mode</strong> - You can only use the credit cards listed here in Test Mode. Valid card numbers are:
   		<br /><br />Master:&nbsp;&nbsp;5431111111111111
      <br />Visa:&nbsp;&nbsp;&nbsp;&nbsp;4111111111111111
      <br />Amex:&nbsp;&nbsp;&nbsp;371111111111114
      <br />Diners:&nbsp;&nbsp;36000000000008
      <br /><br />All Test Cards can use any valid Expiry Date.');

   $form['credit_card']['exp_year']['#description'] = t('<strong>Test Mode</strong> -All Test Cards can use any valid Expiry Date.');

   $form['credit_card']['owner']['#description'] = t('<strong>Test Mode</strong> - PXPost transactions do NOT require the Card Owner field. If you choose to request this information from your users it will however be stored with the transaction records here and in the PXPost system.');

 }
 
  $form['commerce_oz_2p_head']['credit_card'] = $form['credit_card'];
	unset($form['credit_card']);
	
	
  return $form;
  
}

/**
 * Payment method callback: checkout form validation.
 */
function commerce_oz_paymentexpress_2p_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {

  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');

  // Validate the credit card fields.
  $settings = array(
    'form_parents' => array_merge($form_parents, array('credit_card')),
  );

  if (!commerce_payment_credit_card_validate($pane_values['commerce_oz_2p_head']['credit_card'], $settings)) {
    return FALSE;
  }
}

/**
 * Payment method callback: checkout form submission.
 *
 * This is the function that handles the calls to  servers
 */
function commerce_oz_paymentexpress_2p_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {

  // Get the order
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $order_id = $order->order_id;
  
  $credit_card = $pane_values['commerce_oz_2p_head']['credit_card'];
  
	$settings = _commweb_get_var('commerce_oz_gateway_' . $payment_method['gateway_provider']);
	
  $debugmode = $settings['commerce_oz_debug_mode'];
  $livemode = $settings['commerce_oz_transaction_mode'];
  
	// Transaction record created for the transaction id
  $transaction = _commerce_oz_open_transaction($payment_method['base'], $order->order_id);
  
	$request_map = _commerce_oz_pxpost_build_request($transaction, $order, $credit_card, $settings);

	$response_map = _commerce_oz_pxpost_post_request($request_map);
  
	$request_map['CardNumber'] = _commerce_commweb_build_safe_cc_string($request_map['CardNumber']);
	$request_map['PostUsername'] = _commweb_build_merchant_string($request_map['PostUsername']);
	$request_map['PostPassword'] = _commweb_build_accesscode_string($request_map['PostPassword']);
  
  dsm($request_map, '$request_map');
  dsm($response_map, '$response_map');
  
  _commerce_oz_paymentexpress_2p_close_transaction($transaction, $payment_method, $request_map, $response_map);
	
	_commerce_oz_debug_transaction($transaction, $request_map, $response_map, ($debugmode == 'debug'));
	
	// Fire completion Rule
	rules_invoke_event('commerce_oz_transaction_complete', $transaction);
	
	return _commerce_oz_handle_payment_response($transaction, $request_map, $response_map);
 
}





function _commerce_oz_pxpost_build_request($transaction, $order, $credit_card, $settings) {
		
		// Process Expiry Date
		$cc_date =  $credit_card['exp_month'] . substr($credit_card['exp_year'], 2, 2);
		
		//Generate the MerchantTxnRef and the OrderInfo
		$merchant_reference = $order->order_id . '-' . $transaction->transaction_id;

		// Make the request_map array
		$request_map = array(
			'PostUsername' => 	$settings['pxpost_username'],
			'PostPassword' => 	$settings['pxpost_password'],
			'Amount' => 				$order->commerce_order_total['und'][0]['amount'] / 100, //pxpost uses decimal amounts e.g. $12.34
			'InputCurrency' => 	$order->commerce_order_total['und'][0]['currency_code'],
			'CardNumber' => 		$credit_card['number'],
			'DateExpiry' => 		$cc_date,
			'TxnType' => 				'Purchase',
			'MerchantReference' => $merchant_reference,
		);
		
		if($settings['commerce_oz_use_card_name']) {
			$request_map['CardHolderName'] = $credit_card['owner'];
		}

		if($settings['commerce_oz_use_cvv']) {
			$request_map['Cvc2'] = $credit_card['code'];
			$request_map['Cvc2Presence'] = 1; // Check CVV code
		} else {
			$request_map['Cvc2Presence'] = 0; // Don't check CVV code
		}
		
		return $request_map;
}

function _commerce_oz_pxpost_post_request($request_map) {
		
		$request_xml = _commerce_oz_array2xml($request_map, '<Txn/>');
		$request_xml = str_replace('<?xml version="1.0"?>', '', $request_xml);
				
		$URL = "https://sec.paymentexpress.com/pxpost.aspx";
		              
		$ch = curl_init(); 
		curl_setopt($ch, CURLOPT_URL,  $URL);
		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_POSTFIELDS, $request_xml);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0); //Needs to be included if no *.crt is available to verify SSL certificates
		curl_setopt($ch,CURLOPT_SSLVERSION,3);  
		$result = curl_exec ($ch); 
		curl_close ($ch);
		
		return json_decode(json_encode((array) simplexml_load_string($result)),1);;
}



function _commerce_oz_paymentexpress_2p_close_transaction($transaction, $payment_method, $request_map, $response_map) {

// Update the transaction object

	
	
	$transaction->instance_id = $payment_method['base'] . '|commerce_payment_' . $payment_method['base'];
 	$transaction->amount = $response_map['Transaction']['Amount'] * 100;  //PXPost uses decimal values not cent values
  $transaction->currency_code = $response_map['Transaction']['InputCurrencyName'];
  $transaction->payload = array('request' => $request_map, 'response' => $response_map);
  
  $transaction->remote_id = $response_map['DpsTxnRef'];    
  $transaction->remote_status = $response_map['Success'];  
  
  $transaction->status = $transaction->remote_status ? COMMERCE_PAYMENT_STATUS_SUCCESS : COMMERCE_PAYMENT_STATUS_FAILURE;
  
  $transaction->message = t('<strong>Purchase</strong> Code: @code - @message'); 
  
  $trans_message =  $response_map['ResponseText'];
  
  if($transaction->remote_status === COMMERCE_PAYMENT_STATUS_FAILURE)
  {	
  	$transaction->message = t('<strong>Failure</strong> Code: @code - @message'); 
  	$trans_message .= ' - Failed Card: ' . ': ' ; //. $request_map['vpc_CardNum'];
  } 
  
  $transaction->message_variables = array(
  	'@code' => $transaction->status,
  	'@message' => $trans_message,
  		);
  		
  // Save the transaction information.
  commerce_payment_transaction_save($transaction);		
  		
  return $response_map;			
}





