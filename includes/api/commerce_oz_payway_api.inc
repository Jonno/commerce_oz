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
function commerce_oz_payway_api_settings_form($settings = NULL) {

  $form = array();
 
	$logopath = '/' . drupal_get_path('module', 'commerce_oz') . '/image/payway_68.png';
	
	$descText = '<a href="http://www.payway.com.au"><img src="' . $logopath . '" /></a><br />Settings for the <a href="http://www.payway.com.au"/">PayWay</a> payment gateway.<br />No settings are provided here.';

  $form['intro_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('PayWay API'),
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
function commerce_oz_payway_api_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  	 	 	
  // Include the Drupal Commerce credit card file for forms
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');
  
  $form_params = array();
  $form_defaults = array();
  
  $settings = _commweb_get_var('commerce_oz_gateway_' . $payment_method['gateway_provider']);
  
  $livemode = $settings['commerce_oz_transaction_mode'];
  
  // Use CVV Field?
  if($settings['commerce_oz_use_cvv'] == 'cvv')
  {
  	$form_params['code'] = '';
  }
 
  // Use Card Holder Name Field ?
  if($settings['commerce_oz_use_card_name'] == 'name')
  {
  	$form_params['owner'] = 'Card Holder';
  }
 

	$logopath = '/' . drupal_get_path('module', 'commerce_oz') . '/image/payway_68.png';
	
	$descText = '<a href="http://www.payway.com.au"><img src="' . $logopath . '" /></a><br />You are paying through the <a href="http://www.payway.com.au"/">PayWay</a> payment gateway.<br />Enter your payment details below and click Continue to next step.<br />You will be taken to your Receipt information.<br />';

 // set the defaults for testing
 if($settings['commerce_oz_transaction_mode'] == 'test') {

	 	$form_defaults['number'] = $settings['commerce_oz_test_card_number'];
	 	$form_defaults['code'] = $settings['commerce_oz_test_card_cvv'];
	 	$form_defaults['exp_month'] = $settings['commerce_oz_test_card_exp_month'];
	 	$form_defaults['exp_year'] = $settings['commerce_oz_test_card_exp_year'];
	}

 	// Load the DC credit card form
 	$form = commerce_payment_credit_card_form($form_params, $form_defaults);

  $form['commerce_oz_payway'] = array(
    '#type' => 'fieldset',
    '#title' => $payment_method['title'],
    '#collapsible' => FALSE, 
  	'#collapsed' => FALSE,
    '#description' => t($descText),
  );


	// Give the card number field some air
	$form['credit_card']['number']['#size'] = 20;




 if($settings['commerce_oz_transaction_mode'] == 'test')
 {
 
    $form['credit_card']['number']['#description'] = t('<strong>Test Mode</strong> - You can only use the credit cards listed here in Test Mode. Valid card numbers are:
   		<br /><br />Master:&nbsp;&nbsp;5163200000000008 Expiry 08/20 CVN 847
      <br />Visa:&nbsp;&nbsp;&nbsp;&nbsp;4564710000000004 Expiry 02/19 CVN 070
      <br />Amex:&nbsp;&nbsp;&nbsp;371111111111114
      <br />Diners:&nbsp;&nbsp;36000000000008
      <br />');

   $form['credit_card']['exp_year']['#description'] = t('<strong>Test Mode</strong> - All Test Cards can use any valid Expiry Date.');

   $form['credit_card']['owner']['#description'] = t('<strong>Test Mode</strong> - Payway transactions do NOT require the Card Owner field. If you choose to request this information from your users it will however be stored with the transaction records here.');

 }
 
  $form['commerce_oz_payway']['credit_card'] = $form['credit_card'];
	unset($form['credit_card']);
	
	
  return $form;
  
}

/**
 * Payment method callback: checkout form validation.
 */
function commerce_oz_payway_api_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {

	
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');

  // Validate the credit card fields.
  $settings = array(
    'form_parents' => array_merge($form_parents, array('credit_card')),
  );

  if (!commerce_payment_credit_card_validate($pane_values['commerce_oz_payway']['credit_card'], $settings)) {
    return FALSE;
  }
}

/**
 * Payment method callback: checkout form submission.
 *
 * This is the function that handles the calls to  servers
 */
function commerce_oz_payway_api_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
	
	
	$path = drupal_get_path('module', 'commerce_oz');
	require_once($path . '/includes/api/Qvalent_PayWayAPI.php');
	
//	dsm($paywayAPI);
	
  // Get the order
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $order_id = $order->order_id;

  // Get the credit card from the fieldset
  $credit_card = $pane_values['commerce_oz_payway']['credit_card'];
  
  
  watchdog('payway', nl2br(print_r($pane_values, TRUE)));
  
	// Get the payment method settings
  $settings = _commweb_get_var('commerce_oz_gateway_' . $payment_method['gateway_provider']);	
  $debugmode = $settings['commerce_oz_debug_mode'];
  $livemode = $settings['commerce_oz_transaction_mode'];
 
  $path_to_cert = $_SERVER['DOCUMENT_ROOT'] . base_path() . $path . '/includes/api/cacerts.crt';
  
  
  $initParams =
        "certificateFile=" . $settings['certificate_path'] . '&' .
        "caFile=" . $path_to_cert . '&' .
        "logDirectory=" . 'sites/commerce.localhost/files';
    $paywayAPI = new Qvalent_PayWayAPI();
    $paywayAPI->initialise( $initParams );
  
  
  // Create a new transaction for the order
  $transaction = _commerce_oz_open_transaction($payment_method['base'], $order->order_id);
  
  
  //	watchdog('payway', nl2br(print_r($paywayAPI, TRUE)));
  ///	watchdog('payway', nl2br(print_r($initParams, TRUE)));
  //	watchdog('payway', nl2br(print_r($settings, TRUE)));
  	//dsm($initParams);
  	
  	//Generate the MerchantTxnRef and the OrderInfo
		$merchant_reference = $order->order_id . '-' . $transaction->transaction_id;

  	
  	
  	$requestParameters = array();
    $requestParameters[ "order.type" ] = "capture";
    $requestParameters[ "customer.username" ] = $settings['payway_username'];
    $requestParameters[ "customer.password" ] = $settings['payway_password'];
    $requestParameters[ "customer.merchant" ] = $settings['payway_merchant'];
    
    $requestParameters[ "customer.orderNumber" ] = $merchant_reference;
    $requestParameters[ "customer.customerReferenceNumber" ] = $merchant_reference;
    
 //   $requestParameters[ "customer.originalOrderNumber" ] = $order->order_id;
    $requestParameters[ "card.PAN" ] = $credit_card['number'];
    
   
   	if(isset($credit_card['credit_card']['code'])) {
    		$requestParameters[ "card.CVN" ] = $credit_card['code'];
		}
    
      // Use Card Holder Name Field ?
  	if(isset($credit_card['credit_card']['name']))
	  {
	  	$requestParameters['card.cardHolderName'] = $credit_card['name'];
	  }
    
    
    $requestParameters[ "card.expiryYear" ] = substr($credit_card['exp_year'], 2, 2);
    $requestParameters[ "card.expiryMonth" ] = $credit_card['exp_month'];
    $requestParameters[ "card.currency" ] = 'AUD';
    $requestParameters[ "order.amount" ] = $charge['amount'];
    $requestParameters[ "order.ECI" ] = 'SSL';

    $requestText = $paywayAPI->formatRequestParameters( $requestParameters );
  	
  	
  	watchdog('payway', nl2br(print_r($requestParameters, TRUE)));
  	
  	$responseText = $paywayAPI->processCreditCard( $requestText );

    // Parse the response string into an array
    $responseParameters = $paywayAPI->parseResponseParameters( $responseText );

 // 	dsm($responseParameters);
  watchdog('payway', nl2br(print_r($responseParameters, TRUE)));
  
  
	$request_map = $requestParameters;

	$response_map = $responseParameters;
  
	$request_map['customer.username'] = _commweb_build_merchant_string($request_map['customer.username']);
	$request_map['customer.password'] = _commweb_build_accesscode_string($request_map['customer.password']);
	$request_map['card.PAN'] = _commweb_build_accesscode_string($request_map['card.PAN']);
	
	if(isset($credit_card['credit_card']['code'])) {
			$request_map['card.CVN'] = _commweb_build_accesscode_string($request_map['card.CVN']);
	}
 

  
  _commerce_oz_payway_api_close_transaction($transaction, $payment_method, $request_map, $response_map);
	
	_commerce_oz_debug_transaction($transaction, $request_map, $response_map, ($debugmode == 'debug'));
	
	// Fire completion Rule
	rules_invoke_event('commerce_oz_transaction_complete', $transaction);
	
	return _commerce_oz_handle_payment_response($transaction, $request_map, $response_map);
  


}



function _commerce_oz_payway_api_close_transaction($transaction, $payment_method, $request_map, $response_map) {

// Update the transaction object

	
	
	$transaction->instance_id = $payment_method['base'] . '|commerce_payment_' . $payment_method['base'];
 	$transaction->amount = $request_map['order.amount'];
  $transaction->currency_code = $request_map['card.currency'];
  $transaction->payload = array('request' => $request_map, 'response' => $response_map);
  
  if(isset($response_map['response.receiptNo']))
  {
  	$transaction->remote_id = $response_map['response.receiptNo'];  
  } else {
  	$transaction->remote_id = 0;
  }
  
   
  $transaction->remote_status = $response_map['response.summaryCode'];  // 0, 1, 2, 3
  $transaction->status = $transaction->remote_status ? COMMERCE_PAYMENT_STATUS_FAILURE : COMMERCE_PAYMENT_STATUS_SUCCESS;
  
  $transaction->message = t('<strong>Purchase</strong> Code: @code - @message'); 
  
  $trans_message =  $response_map['response.text'];
  
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





