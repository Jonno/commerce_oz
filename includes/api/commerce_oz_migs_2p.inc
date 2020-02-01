<?php

/**
 * @file
 * Implements CommWeb Virtual Payment Service for use in Drupal Commerce.
 */



/**
 * Payment method callback: settings form.
 */
function commerce_oz_migs_2p_settings_form($settings = NULL) {

  $form = array();
  
  // Add some defaults.
  $settings = (array)$settings + array(
    'commerce_commweb_currency' => 'AUD'
  );

	$logopath = '/' . drupal_get_path('module', 'commerce_commweb') . '/image/cba_logo.gif';
	
	// // dsm($settings);
	
	$descText = '<a href="http://www.commbank.com.au"><img src="' . $logopath . '" /></a><br />Integrates the <a href="http://www.commbank.com.au/business/merchant-services/payment-options/payments-via-your-website/default.aspx">CommWeb VPC for Merchant Hosted Payment</a> (2 party) provided by <a href="http://www.commbank.com.au">Commonwealth Bank</a> (Australia).<br />No settings are provided here.';

  $form['commerce_commweb_intro_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('CommWeb VPC'),
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
function commerce_oz_migs_2p_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  
//	// dsm($payment_method, '$payment_method');
//	// dsm($checkout_pane);
//	// dsm($pane_values);
//	// dsm($order);
  	 	 	
  // Include the Drupal Commerce credit card file for forms
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');
  
	$settings = _commweb_get_var('commerce_oz_gateway_' . $payment_method['gateway_provider']);
 
// 	dsm($settings, '$settings');

  $form_params = array();
  $form_defaults = array();
  
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
 
  
 // set the defaults for testing
 if($settings['commerce_oz_transaction_mode'] == 'test')
 {

		 	$form_defaults['number'] = $settings['commerce_oz_test_card_number'];
		 	$form_defaults['code'] = $settings['commerce_oz_test_card_cvv'];
		 	$form_defaults['exp_month'] = $settings['commerce_oz_test_card_exp_month'];
		 	$form_defaults['exp_year'] = $settings['commerce_oz_test_card_exp_year'];
		}
		

 $gateway = commerce_oz_load_gateway_provider($payment_method['gateway_provider']);
 
 	$form = commerce_payment_credit_card_form($form_params, $form_defaults);

	$logopath = '/' . drupal_get_path('module', 'commerce_oz') . '/image/' . $payment_method['gateway_provider']  .  '_68.png';
	
	$descText = '<a href="'  . $gateway['owner_website'] .  '"><img src="' . $logopath . '" /></a><br />Enter your payment details below and Click Continue.<br />On completing your transaction, you will be taken to your Receipt information.<br />';

  $form['commweb_3p_head'] = array(
    '#type' => 'fieldset',
    '#title' => t($payment_method['title']),
    '#collapsible' => FALSE, 
  	'#collapsed' => FALSE,
    '#description' => t($descText),
  );
 

 if($settings['commerce_oz_transaction_mode'] == 'test')
 {
  // // dsm($form);
   
   
   $form['credit_card']['number']['#description'] = t('<strong>Test Mode</strong> - You can only use the credit cards listed here to test in Test Mode. Valid card numbers are:
   		<br /><br />Master:&nbsp;&nbsp;5123456789012346
      <br />Visa:&nbsp;&nbsp;&nbsp;&nbsp;4987654321098769
      <br />Amex:&nbsp;&nbsp;&nbsp;345678901234564
      <br />Diners:&nbsp;&nbsp;30123456789019
      <br /><br />All CommWeb Test Cards must use 05-2013 as the Expiry Date.');

   $form['credit_card']['exp_year']['#description'] = t('<strong>est Mode</strong> - All CommWeb Test Cards must use 05-2013 as the Expiry Date');

   $form['credit_card']['owner']['#description'] = t('<strong>Test Mode</strong> - Transactions do NOT require or use the Card Owner field. If you choose to request this information from your users it will however be stored with the transaction record.');

  
   
   // Provide a textfield to pass different Amounts to MIGS 
 $form['credit_card']['test_amount'] = array(
     '#input' => TRUE,

    '#type' => 'textfield',
    '#size' => 6,
    '#maxlength' => 10,
   		
    '#title' => t('Test Mode - Custom Amount'), 
    '#default_value' => $order->commerce_order_total['und'][0]['amount'],
    '#disabled'  => FALSE,
    '#description' => t('<strong>Test Mode</strong> - Update the Amount (in cents) sent for processing to change your desired transaction response.<br /> Valid amounts are <br />xxx00 = Approved
    <br />xxx05 = Declined. Contact Bank.
    <br />xxx10 = Transaction could not be processed.
    <br />xxx33 = Expired Card.
    <br />xxx50 = Unspecified Failure.
    <br />xxx51 = Insufficient Funds.
    <br />xxx68 = Communications failure with bank'),
    '#required' => FALSE, 
    );  
       
 }
 
 
 	$form['commweb_3p_head']['credit_card'] = $form['credit_card'];
	unset($form['credit_card']);
	
//	// dsm($form);
	
  return $form;
  
}

/**
 * Payment method callback: checkout form validation.
 */
function commerce_oz_migs_2p_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {

  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');

  // Validate the credit card fields.
  $settings = array(
    'form_parents' => array_merge($form_parents, array('credit_card')),
  );

  if (!commerce_payment_credit_card_validate($pane_values['commweb_3p_head']['credit_card'], $settings)) {
    return FALSE;
  }
}

/**
 * Payment method callback: checkout form submission.
 *
 * This is the function that handles the calls to CommWeb servers
 */
function commerce_oz_migs_2p_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
 
	dsm($payment_method);
 	/* dsm($pane_form);
  	 	// dsm($pane_values);
  	 	 	// dsm($order);
  	 	 	// dsm($charge);
  */
  	 	 	
  // Get the order
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $order_id = $order->order_id;
  
  $pane_values['credit_card'] = $pane_values['commweb_3p_head']['credit_card'];
 // // dsm($pane_values);
  
  // Get the settings and set some vars for same
 // $settings = $payment_method['settings'];
  $debugmode = 'debug'; //$settings['commerce_commweb_vpc_settings']['commerce_commweb_vpc_debug'];
  $livemode = 'test'; //$settings['commerce_commweb_vpc_settings']['commerce_commweb_vpc_mode'];
  
	// Transaction record
	// Prepare a transaction object to log the API response.
  $transaction = commerce_payment_transaction_new($payment_method['base'], $order->order_id);
  
    // Save the transaction information for a fresh transaction id.
  commerce_payment_transaction_save($transaction);
  
	// BUILD THE POST DATA
	// Use Test Amount if necessary
	if(($pane_values['credit_card']['test_amount']) && ($livemode == 'test'))
	{	$amount = $pane_values['credit_card']['test_amount'];
		
	} else {	$amount = $charge['amount'];
		
	}
	
	$gateway_key = $payment_method['gateway_provider'];
	
	
  $request_string = _commerce_oz_build_migs_request_payload($order, $transaction, $pane_values, $amount, $gateway_key); 
//	// dsm($request_string, '$request_string');
	
	$request_map = _commerce_oz_process_vpc_request($request_string);
//	// dsm($request_map, '$request_map');

	// VPC curl function
	$response_string = _commerce_oz_post_migs_request($request_string);
	
	// Map curl response to array and update transaction object
	$response_map = _commerce_oz_process_vpc_response($transaction, $payment_method, $response_string, $request_map);
	
		// Debug information
  if($debugmode == 'debug')
  {
  
  	dpm($request_map);
  	watchdog('commerce_oz', 'Request Data was: @requestdata', array('@requestdata' => $request_string), WATCHDOG_DEBUG);

  
		dpm($response_map);
  	watchdog('commerce_oz', 'Response Map: @returndata', array('@returndata' => print_r($response_map, true)), WATCHDOG_DEBUG);
  	
  }

  // Save the transaction information.
  commerce_payment_transaction_save($transaction);
	
	// Fire completion Rule
	rules_invoke_event('commerce_oz_transaction_complete', $transaction);

  // If the payment failed, display an error and rebuild the form.
  if ($transaction->status === COMMERCE_PAYMENT_STATUS_FAILURE) 
  {
  
    drupal_set_message(t('There was an error processing your card. <br />Check your information or try a different card.'), 'error');
    
    drupal_set_message('Response: ' . $response_map['vpc_TxnResponseCode'] . ' - Message: "' . $response_map['vpc_Message'] . '"', 'error');
    
    watchdog('commerce_oz', 
    					'Failed - Request: @requestdata2', 
    					array('@requestdata2' => $request_map), WATCHDOG_WARNING);
    
    watchdog('commerce_oz', 
    					'Failed - Response: @requestdata3', 
    					array('@requestdata3' => $response_map), WATCHDOG_WARNING);
    
    	// Fire failure Rule
   rules_invoke_event('commerce_oz_transaction_fail', $transaction);
    
    return FALSE;
    
  } else { 
  
  	// Fire success Rule
  	rules_invoke_event('commerce_oz_transaction_success', $transaction);
  	
  	return TRUE;
  
  }

}



/**
 * @param $order
 *   	Order object
 * @param $amount
 *   	The amount in dollar value
 * @param $merchantid
 *   	Merchant ID
 * @param $accesscode
 *   	Access Code
 * @param $panevalues
 *   	Pane Values from the cc form
 * @param $testmode
 *   	optional - contains string 'TEST' for test mode
 *
 * @return
 *   	$postdata suitable for CURL.
 */

function _commerce_oz_build_migs_request_payload($order, $transaction, $credit_card, $amount, $gateway_key) 
{
		// Get the API Credentials
		
		$settings = _commweb_get_var('commerce_oz_gateway_' . $gateway_key);
		
		$merchantid = $settings['merchant_id'];
		$accesscode = $settings['access_code'];
	
		// create a variable to hold the POST data
		$postData = '';
		$amp = '&';
		
		// Reverse the CC Date to a 4 digit string in YYMM format
		$cc_date =  substr($credit_card['credit_card']['exp_year'], 2, 2) . $credit_card['credit_card']['exp_month'];
		
		//Generate the MerchantTxnRef and the OrderInfo
		$merchTxnRef = $order->order_id . '-' . $transaction->transaction_id;
		
		// Assemble the string
		$postData .= 'vpc_Version' . '=' . COMMERCE_COMMWEB_vpc_Version . $amp;
		$postData .= 'vpc_Command' . '=' . COMMERCE_COMMWEB_vpc_Command . $amp;
		$postData .= 'vpc_MerchTxnRef' . '=' . $merchTxnRef . $amp;
		$postData .= 'vpc_AccessCode' . '=' . $accesscode . $amp;
		$postData .= 'vpc_Merchant' . '=' . $merchantid . $amp; 
		$postData .= 'vpc_OrderInfo' . '=' . $order->order_id . $amp;
		$postData .= 'vpc_Amount' . '=' . $amount . $amp;
		$postData .= 'vpc_CardNum' . '=' . $credit_card['credit_card']['number'] . $amp;
		$postData .= 'vpc_CardExp' . '=' . $cc_date;
		
		if(isset($credit_card['credit_card']['code'])) {
				$postData .= $amp . 'vpc_CardSecurityCode' . '=' . $credit_card['credit_card']['code'];
		}
		
		return $postData;
		
}


function _commerce_oz_process_vpc_request($request_string)
{
	  $request_map = array();
		parse_str($request_string, $request_map);
		$request_map['vpc_CardNum'] = _commerce_commweb_build_safe_cc_string($request_map['vpc_CardNum']);
		$request_map['vpc_Merchant'] = _commweb_build_merchant_string($request_map['vpc_Merchant']);
		$request_map['vpc_AccessCode'] = _commweb_build_accesscode_string($request_map['vpc_AccessCode']);
		return $request_map;
}

function _commerce_oz_process_vpc_request_map2($request_map)
{
		$request_map['vpc_Merchant'] = _commweb_build_merchant_string($request_map['vpc_Merchant']);
		$request_map['vpc_AccessCode'] = _commweb_build_accesscode_string($request_map['vpc_AccessCode']);
		return $request_map;
}



function _commerce_oz_process_vpc_response($transaction, $payment_method, $response_string, $request_map) {
	// Map VPC repsonse to array
	$response_map = array();
	parse_str($response_string, $response_map);
	$response_map['vpc_Merchant'] = _commweb_build_merchant_string($response_map['vpc_Merchant']);
	
	// Update the transaction object
  $transaction->instance_id = $payment_method['base'] . '|commerce_payment_' . $payment_method['base'];
  $transaction->amount = $response_map['vpc_Amount'];
  $transaction->currency_code = 'AUD';
  $transaction->payload = array('request' => $request_map, 'response' => $response_map);
  
  $transaction->remote_id = $response_map['vpc_TransactionNo'];    
  $transaction->remote_status = $response_map['vpc_TxnResponseCode'];  
  
  $transaction->status = $response_map['vpc_TxnResponseCode'] ? COMMERCE_PAYMENT_STATUS_FAILURE : COMMERCE_PAYMENT_STATUS_SUCCESS;
  
  $transaction->message = $test_message . t('<strong>Purchase</strong> Result: @code - @message'); 
  
  $transaction_message =  $response_map['vpc_Message'];
  
  if($response_map['vpc_TxnResponseCode'])
  {	
  	$transaction->message = t('<strong>Failed</strong> Code: @code - @message'); 
  	$transaction_message .= ' - Failed Card: ' . $request_map['vpc_CardNum']; 
  	// _commweb_map_code_to_cardtype($response_map['vpc_Card']) . ': ' . $request_map['vpc_CardNum'];
  } 
  
  $transaction->message_variables = array(
  	'@code' => $response_map['vpc_TxnResponseCode'],
  	'@message' => $transaction_message,
  		);
  return $response_map;			
}


/**
 * @param $post_data
 *   POST data returned from build function
 *
 *	 POST is handled by CURL with output buffering
 *
 * @return
 *   response data (Array)
 */

function _commerce_oz_post_migs_request($post_data)
{
	ob_start();
	
	// initialise Client URL object
	$curlhandle = curl_init();
	
	// set the URL of the VPC
	curl_setopt ($curlhandle, CURLOPT_URL, COMMERCE_COMMWEB_URL);
	curl_setopt ($curlhandle, CURLOPT_POST, 1);
	curl_setopt ($curlhandle, CURLOPT_POSTFIELDS, $post_data);
	curl_setopt ($curlhandle, CURLOPT_SSL_VERIFYPEER, 0);
	curl_setopt ($curlhandle, CURLOPT_SSL_VERIFYHOST, 0);
	
	// connect
	curl_exec ($curlhandle);
	
	// get response
	$response = ob_get_contents();
	
	// turn output buffering off.
	ob_end_clean();
	
	return $response;

}


