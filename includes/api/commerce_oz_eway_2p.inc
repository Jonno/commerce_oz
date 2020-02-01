<?php

/**
 * @file
 * Implements eWay Direct for use in Drupal Commerce.
 */

define('COMMERCE_OZ_EWAY_2P_LIVE_URL', 'https://www.eway.com.au/gateway_cvn/xmlpayment.asp');
define('COMMERCE_OZ_EWAY_2P_TEST_URL', 'https://www.eway.com.au/gateway_cvn/xmltest/testpage.asp');

/**
 * Payment method callback: settings form.
 */
function commerce_oz_eway_2p_settings_form($settings = NULL) {

  $form = array();
  

	$logopath = '/' . drupal_get_path('module', 'commerce_oz') . '/image/eway_68.png';
	
	$descText = '<a href="http://www.eway.com.au"><img src="' . $logopath . '" align ="left" /></a>Payment Method for the <a href="http://www.eway.com.au/how-it-works/what-products-are-included-">eWay Direct</a> (2 party) provided by <a href="http://www.eway.com.au/company">eWay Payment Service</a> (Australia).<br />No settings are provided here.';

  $form['commerce_oz_intro_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('eWay Direct'),
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
function commerce_oz_eway_2p_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  	 	 	
  // Include the Drupal Commerce credit card file for forms
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');
  
  $form_params = array();
  $form_defaults = array();
  
  $settings = $payment_method['settings'];
  
  $livemode = 'live'; //$settings['commerce_commweb_vpc_settings']['commerce_commweb_vpc_mode'];
	
	$form_params['owner'] = 'Card Holder';	
 	$form_params['code'] = 'CVV';
 	$form = commerce_payment_credit_card_form($form_params, $form_defaults);

	$logopath = '/' . drupal_get_path('module', 'commerce_oz') . '/image/eway_68.png';
	
	$descText = '<a href="http://www.eway.com.au"><img src="' . $logopath . '" /></a><br />Enter your payment details below and Click Continue.<br />On completing your transaction, you will be taken to your Receipt information.<br />';

  $form['commerce_oz_2p_head'] = array(
    '#type' => 'fieldset',
    '#title' => $payment_method['title'],
    '#collapsible' => FALSE, 
  	'#collapsed' => FALSE,
    '#description' => t($descText),
  );
 

 if($livemode == 'test')
 {
  // // dsm($form);
   
   
   $form['credit_card']['number']['#description'] = t('<strong>CommWeb Test Mode</strong> - You can only use the credit cards listed here to test with CommWeb VPC in Test Mode. Valid card numbers are:
   		<br /><br />Master:&nbsp;&nbsp;5123456789012346
      <br />Visa:&nbsp;&nbsp;&nbsp;&nbsp;4987654321098769
      <br />Amex:&nbsp;&nbsp;&nbsp;345678901234564
      <br />Diners:&nbsp;&nbsp;30123456789019
      <br /><br />All CommWeb Test Cards must use 05-2013 as the Expiry Date.');

   $form['credit_card']['exp_year']['#description'] = t('<strong>CommWeb Test Mode</strong> - All CommWeb Test Cards must use 05-2013 as the Expiry Date');

   $form['credit_card']['owner']['#description'] = t('<strong>CommWeb Test Mode</strong> - CommWeb transactions do NOT require or use the Card Owner field. If you choose to request this information from your users it will however be stored with the transaction record.');

   
   
   /* Provide a textfield to pass different Amounts */
 $form['credit_card']['test_amount'] = array(
     '#input' => TRUE,

    '#type' => 'textfield',
    '#size' => 6,
    '#maxlength' => 10,
   		
    '#title' => t('CommWeb Test Mode - Custom Amount'), 
    '#default_value' => $order->commerce_order_total['und'][0]['amount'],
    '#disabled'  => FALSE,
    '#description' => t('<strong>CommWeb Test Mode</strong> - Update the Amount (in cents) sent for processing to change your desired transaction response from CommWeb.<br /> Valid amounts are <br />xxx00 = Approved
    <br />xxx05 = Declined. Contact Bank.
    <br />xxx10 = Transaction could not be processed.
    <br />xxx33 = Expired Card.
    <br />xxx50 = Unspecified Failure.
    <br />xxx51 = Insufficient Funds.
    <br />xxx68 = Communications failure with bank'),
    '#required' => FALSE, 
    );  
       
 }
 
 
 	$form['commerce_oz_2p_head']['credit_card'] = $form['credit_card'];
	unset($form['credit_card']);
	
	
  return $form;
  
}

/**
 * Payment method callback: checkout form validation.
 */
function commerce_oz_eway_2p_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {

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
function commerce_oz_eway_2p_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
 
 // // dsm($payment_method, 'method');
// 	// dsm($pane_form, 'pane');
 //	// dsm($pane_values, 'values');
 // // dsm($order, 'order');
 // // dsm($charge, 'charge');
  
  	 	 	
  // Get the order
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $order_id = $order->order_id;
  
  $credit_card = $pane_values['commerce_oz_2p_head']['credit_card'];
 // // dsm($pane_values);
  
  // Get the settings and set some vars for same
 // $settings = $payment_method['settings'];
  $debugmode = 'debug'; //$settings['commerce_commweb_vpc_settings']['commerce_commweb_vpc_debug'];
  $livemode = 'test'; //$settings['commerce_commweb_vpc_settings']['commerce_commweb_vpc_mode'];
  
	// Transaction record
	// Prepare a transaction object to log the API response.
  $transaction = _commerce_oz_open_transaction($payment_method['base'], $order->order_id);
  
  module_load_include('php', 'commerce_oz', 'includes/api/EwayPayment');

	$eway = new EwayPayment( '91063492', 'https://www.eway.com.au/gateway_cvn/xmltest/testpage.asp' );
	
	// Card Details
	$eway->setCardHoldersName( $credit_card['owner'] );
	$eway->setCardNumber( $credit_card['number'] );
	$eway->setCardExpiryMonth( $credit_card['exp_month'] );
	$eway->setCardExpiryYear( substr($credit_card['exp_year'], -2) );
	$eway->setTotalAmount( $charge['amount'] );
//	$eway->setTotalAmount	(1000);

	$eway->setCVN( $credit_card['code'] );
	
	// Invoice & Transaction Details
	
	$eway->setCustomerInvoiceDescription( 'Testing' );
	$eway->setCustomerInvoiceRef( $order_id );
	$eway->setTrxnNumber( $transaction->transaction_id );

  
  $request_map = array_filter((array)$eway);
  
  $error = $eway->doPayment();
  
  $response_map = array_filter((array)$eway);
  
  // dsm($request_map, '$request_map');
  // dsm($response_map, '$response_map');
  
  _commerce_oz_eway_2p_close_transaction($transaction, $request_map, $response_map);
	
	_commerce_oz_debug_transaction($transaction, $request_map, $response_map, TRUE);
	
	// Fire completion Rule
	rules_invoke_event('commerce_oz_transaction_complete', $transaction);
	
	return _commerce_oz_handle_payment_response($transaction, $request_map, $response_map);
 
}




function _commerce_oz_eway_2p_close_transaction($transaction, $request_map, $response_map) {

	// Update the transaction object
//  $transaction->instance_id = 'commerce_commweb|commerce_payment_commerce_commweb';
 	$transaction->amount = $response_map['ewayResultReturnAmount'];
  $transaction->currency_code = 'AUD';
  $transaction->payload = array('request' => $request_map, 'response' => $response_map);
  
  $transaction->remote_id = $response_map['ewayResultTrxnNumber'];    
  $transaction->remote_status = ($response_map['ewayResultTrxnStatus'] == 'True');  
  
  $transaction->status = $transaction->remote_status ? COMMERCE_PAYMENT_STATUS_SUCCESS : COMMERCE_PAYMENT_STATUS_FAILURE;
  
  $transaction->message = t('<strong>Purchase</strong> Code: @code - @message'); 
  
  $trans_message =  $response_map['ewayResultTrxnError'];
  
  if($transaction->remote_status === COMMERCE_PAYMENT_STATUS_FAILURE)
  {
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





