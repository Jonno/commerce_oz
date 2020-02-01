<?php

/**
 * @file
 * Implements eWay Direct for use in Drupal Commerce.
 */

define('COMMERCE_OZ_EWAY_3P_REQUEST_URL', 'https://au.ewaygateway.com/Request/');
define('COMMERCE_OZ_EWAY_3P_RESULT_URL', 'https://au.ewaygateway.com/Result/');




/**
 * Payment method callback: checkout form.
 * loading the default form from Commerce Payment module
 */
function commerce_oz_eway_3p_redirect_form($form, &$form_state, $order, $payment_method) {

	$logopath = '/' . drupal_get_path('module', 'commerce_oz') . '/image/eway_68.png';
	
	$descText = '<br /><a href="http://www.eway.com.au"><img src="' . $logopath . '" /></a><br /><br />Click the \'Proceed to eWay\' button to complete payment of your purchase at eWay.<br />On completing your transaction, you will be taken to your Receipt information.<br />';



  $form['commerce_oz_3p_head'] = array(
    '#type' => 'fieldset',
    '#title' => $payment_method['title'],
    '#collapsible' => FALSE, 
  	'#collapsed' => FALSE,
    '#description' => t($descText),
  );
 	
 	  // Get the order
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $order_id = $order->order_id;
 	
 	$transaction = _commerce_oz_open_transaction($payment_method, $order->order_id);
 	
 	$request_map = _commerce_oz_eway_3p_build_request($transaction, $payment_method, $order) ;
 	
//	// dsm($request_map, '$request_map');
	$request_string = http_build_query($request_map);
//	// dsm(COMMERCE_OZ_EWAY_3P_REQUEST_URL . '?' . $request_string, '$$request_string');
	
	$ch = curl_init();
	curl_setopt($ch, CURLOPT_URL, COMMERCE_OZ_EWAY_3P_REQUEST_URL . '?' . $request_string);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
	curl_setopt($ch, CURLOPT_HEADER, 1);
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
	
	$response = curl_exec($ch);

	$responsemode = _commerce_oz_fetch_data($response, '<result>', '</result>');
  $responseurl = _commerce_oz_fetch_data($response, '<uri>', '</uri>');
	// dsm($responseurl, '$responseurl');
	   
	$form['#action'] = $responseurl;
	
	$form_state['redirect'] = 'commerce_oz/eway/response';
	
  $form['commerce_oz_3p_head']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Proceed to eWay'),
  );

//	// dsm($form);
//	// dsm($form_state);
		
  return $form;
  
}


/**
 * Payment method callback: checkout form validation.
 */
function commerce_oz_eway_3p_redirect_form_validate($order, $payment_method) {

//	// dsm($_POST, 'POST');

// https://au.ewaygateway.com/Result/?CustomerID=87654321&UserName=TestAccount&AccessPaymentCode=611a5cabc19330f52f9db09e4549c225dda64a71aa8775f53cafce75c0acff0b611a5cabc19330f52f9db09e4549c225dda64a71aa8775f5asdfalkji323jlJS

// https://au.ewaygateway.com/Result/?CustomerID=87654321&UserName=TestAccount&AccessPaymentCode=rSG8OkiqSEIQh7qOV4O9QpPqsypmbGjDLI663n8t6NFFst1rD3

	$eway_settings = _commweb_get_var('commerce_oz_gateway_eway');

	$request_string = '?CustomerID=' . $eway_settings['eway_customer_id'];
	$request_string .= '&UserName=' . $eway_settings['eway_customer_login'];
	$request_string .= '&AccessPaymentCode=' . $_POST['AccessPaymentCode'];
	
	// dsm(COMMERCE_OZ_EWAY_3P_RESULT_URL . $request_string, 'request str');

	$ch = curl_init();
	curl_setopt($ch, CURLOPT_URL, COMMERCE_OZ_EWAY_3P_RESULT_URL . $request_string);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
	curl_setopt($ch, CURLOPT_HEADER, 1);
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
	
	$response = curl_exec($ch);

	// dsm($response, '$response');

	return TRUE;

}

/**
 * Payment method callback: checkout form submission.
 *
 * This is the function that handles the calls to  servers
 */
function commerce_oz_eway_3p_redirect_form_submit($order, $payment_method) {
 
  // dsm($payment_method, 'method');
// 	// dsm($pane_form, 'pane');
// 	// dsm($pane_values, 'values');
  // dsm($order, 'order');
//  // dsm($charge, 'charge');
 
 // dsm($_POST, 'post'); 
// // dsm($order, '$order'); 
// // dsm($payment_method, '$payment_method'); 
  
  if($_POST){
//  return;
  
  }
  	 	 	
  // Get the order
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $order_id = $order->order_id;
 
 /*
  
  $credit_card = $pane_values['commerce_oz_3p_head']['credit_card'];
 // // dsm($pane_values);
  
  // Get the settings and set some vars for same
 // $settings = $payment_method['settings'];
  $debugmode = 'debug'; //$settings['commerce_commweb_vpc_settings']['commerce_commweb_vpc_debug'];
  $livemode = 'test'; //$settings['commerce_commweb_vpc_settings']['commerce_commweb_vpc_mode'];
  
	// Transaction record
	// Prepare a transaction object to log the API response.
  $transaction = _commerce_oz_open_transaction($payment_method['base'], $order->order_id);
  
  module_load_include('php', 'commerce_oz', 'includes/api/EwayPayment');

	$eway = new EwayPayment( '87654321', 'https://www.eway.com.au/gateway_cvn/xmltest/testpage.asp' );
	
	// Card Details
	
	$eway->setCardNumber( $credit_card['number'] );
	$eway->setCardExpiryMonth( $credit_card['exp_month'] );
	$eway->setCardExpiryYear( substr($credit_card['exp_year'], -2) );
//	$eway->setTotalAmount( $charge['amount'] );
	$eway->setTotalAmount	(1000);

	$eway->setCVN( $credit_card['code'] );
	
	// Invoice & Transaction Details
	
	$eway->setCustomerInvoiceDescription( 'Testing' );
	$eway->setCustomerInvoiceRef( $order_id );
	$eway->setTrxnNumber( $transaction->transaction_id );

  
  $request_map = array_filter((array)$eway);
  
  $error = $eway->doPayment();
  
  $response_map = array_filter((array)$eway);
  */
  
//  // dsm($request_map);
//  // dsm($response_map);
  
 // _commerce_oz_eway_3p_close_transaction($transaction, $request_map, $response_map);
	
//	_commerce_oz_debug_transaction($transaction, $request_map, $response_map, TRUE);
	
	// Fire completion Rule
	//rules_invoke_event('commerce_oz_transaction_complete', $transaction);
	
	// return _commerce_oz_handle_payment_response($transaction, $request_map, $response_map);
 
}




function _commerce_oz_eway_3p_close_transaction($transaction, $request_map, $response_map) {

	// Update the transaction object
  $transaction->instance_id = 'commerce_commweb|commerce_payment_commerce_commweb';
 	$transaction->amount = $response_map['ewayResultReturnAmount'];
  $transaction->currency_code = 'AUD';
  $transaction->payload = array('request' => $request_map, 'response' => $response_map);
  
  $transaction->remote_id = $response_map['ewayResultTrxnNumber'];    
  $transaction->remote_status = ($response_map['ewayResultTrxnStatus'] == 'True');  
  
  $transaction->status = $transaction->remote_status ? COMMERCE_PAYMENT_STATUS_SUCCESS : COMMERCE_PAYMENT_STATUS_FAILURE;
  
  $transaction->message = t('<strong>Purchase</strong> Code: @code - @message'); 
  
  $trans_message =  $response_map['ewayResultTrxnError'];
  
  if($transaction->remote_status != 1)
  {
  	$trans_message .= ' - Failed Card: ' . ': ' ; //. $request_map['vpc_CardNum'];
  } 
  
  $transaction->message_variables = array(
  	'@code' => $response_map['ewayResultTrxnNumber'],
  	'@message' => $trans_message,
  		);
  		
  // Save the transaction information.
  commerce_payment_transaction_save($transaction);		
  		
  return $response_map;			
}






/**
 * Payment method callback: settings form.
 */
function commerce_oz_eway_3p_settings_form($settings = NULL) {

  $form = array();
  

	$logopath = '/' . drupal_get_path('module', 'commerce_oz') . '/image/eway_68.png';
	
	$descText = '<a href="http://www.eway.com.au"><img src="' . $logopath . '" align ="left" /></a>Payment Method for the <a href="http://www.eway.com.au/how-it-works/what-products-are-included-">eWay Shared</a> (3 party) provided by <a href="http://www.eway.com.au/company">eWay Payment Service</a> (Australia).<br />No settings are provided here.';

  $form['commerce_oz_intro_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('eWay Shared'),
    '#collapsible' => FALSE, 
  	'#collapsed' => FALSE,
    '#description' => t($descText),
  );

  return $form;
}


function _commerce_oz_eway_3p_build_request($transaction, $payment_method, $order) {

	$eway_settings = _commweb_get_var('commerce_oz_gateway_eway');
	  // Get the order
 // $order_wrapper = entity_metadata_wrapper('commerce_order', $order_id);
 // $order_id = $order->order_id;


  $settings = array(
    // Return to the previous page when payment is canceled
    'cancel_return' => url('checkout/' . $order->order_id . '/payment/back/' . $order->data['payment_redirect_key'], array('absolute' => TRUE)),

    // Return to the payment redirect page for processing successful payments
    'return' => url('checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key'], array('absolute' => TRUE)),

  );

	$request_map = array(
		'CustomerID' => $eway_settings['eway_customer_id'],
		'UserName' => 'AUD',
		'Amount' => '100.00', // $order->amount,
		'Currency' => 'AUD',		
		'PageTitle' => 'Commerce Oz',
	  'PageDescription' => 'Buy Amazing things from us',
		'PageFooter'	 => 'Contact us: 1300 amazing',
		'Language' => 'EN',		
		'CompanyName' => 'Commerce Oz Pty Ltd',
		'CustomerFirstName' => '',
	  'CustomerLastName'	 => '',
		'CustomerAddress' => '',
		'CustomerCity' => '',
		'CustomerState' => '',
		'CustomerPostCode' => '',
		'CustomerCountry' => '',	
		'CustomerEmail' => '',
		'CustomerPhone' => '',	
		'InvoiceDescription' => '',
		'CancelURL' => url('checkout/' . $order->order_id . '/payment/back/' . $order->data['payment_redirect_key'], array('absolute' => TRUE)),
		'ReturnUrl' => url('checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key'], array('absolute' => TRUE)),
		'CompanyLogo' => 'http://www.devel.lawnsale.com.au/eways/commerce_oz_70.png',
		'PageBanner' => '',
		'MerchantReference' => $transaction->transaction_id,
		'MerchantInvoice' => $order->order_id,
		'MerchantOption1' => '',
		'MerchantOption2' => '',
		'MerchantOption3' => '',
		'ModifiableCustomerDetails' => 'false',
		'UseAVS' => 'false',
		'UseZIP' => 'false',
		
	
	);
	
	return $request_map;

}






function _commerce_oz_fetch_data($string, $start_tag, $end_tag)
		{
			$position = stripos($string, $start_tag);  
			$str = substr($string, $position);  		
			$str_second = substr($str, strlen($start_tag));  		
			$second_positon = stripos($str_second, $end_tag);  		
			$str_third = substr($str_second, 0, $second_positon);  		
			$fetch_data = trim($str_third);		
			return $fetch_data; 
		}









