<?php

/**
 * @file
 * Implements PaymentExpress PXPay (Server Hosted) for use in Drupal Commerce.
 */




/**
 * Payment method callback: checkout form.
 * loading the default form from Commerce Payment module
 */
function commerce_oz_paymentexpress_3p_redirect_form($form, &$form_state, $order, $payment_method) {
	$logopath = '/' . drupal_get_path('module', 'commerce_oz') . '/image/ematters_68.png';
	
	$descText = '<br /><a href="http://www.ematters.com"><img src="' . $logopath . '" /></a><br /><br />Click the \'Proceed to eWay\' button to complete payment of your purchase at PaymentExpress.<br />On completing your transaction, you will be taken to your Receipt information.<br />';



  $form['pxpay_3p_head'] = array(
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
 	
 	$request_map = _commerce_oz_pxpay_3p_build_request($transaction, $payment_method, $order);
 	
 //	dsm($order);

 	
 //	dsm($request_map);
 	
	$uri_response = _commerce_oz_pxpay_post_request($request_map);
	  
	dsm($uri_response, 'URI');   

	$uri = str_replace('&amp;', '&', $uri_response['URI']);
	  dsm($uri, '$uri');   

	$form['#action'] = $uri;
	
	//$form_state['redirect'] = 'commerce_oz/paymentexpress/response';
	
  $form['pxpay_3p_head']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Proceed to PaymentExpress'),
  );

//	drupal_goto($uri_response['URI']);
//	// dsm($form);
//	// dsm($form_state);
		
  return $form;
  
}


/**
 * Payment method callback: checkout form validation.
 */
function commerce_oz_paymentexpress_3p_redirect_form_validate($order, $payment_method) {

return TRUE;

	$settings = _commweb_get_var('commerce_oz_gateway_paymentexpress');

	$request_string = '?CustomerID=' . $settings['eway_customer_id'];
	$request_string .= '&UserName=' . $settings['eway_customer_login'];
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
function commerce_oz_paymentexpress_3p_redirect_form_submit($order, $payment_method) {

 
 
}




function _centValueToDecimalString($cents) {

return sprintf("%01.2f", $cents/100);

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


function _commerce_oz_pxpay_3p_build_request($transaction, $payment_method, $order, $trans_type = 'Purchase') {

	$settings = _commweb_get_var('commerce_oz_gateway_paymentexpress');
	
	//Generate the MerchantTxnRef and the OrderInfo
	$merchTxnRef = $order->order_id . '-' . $transaction->transaction_id;
	
	$request_map = array();
	
	$request_map['PxPayUserId'] = $settings['pxpay_username']; 
	$request_map['PxPayKey'] = $settings['pxpay_password'];
	$request_map['MerchantReference'] = $merchTxnRef;	
	$request_map['TxnType']  = $trans_type;
	$request_map['AmountInput'] = _centValueToDecimalString($order->commerce_order_total['und'][0]['amount']);
	$request_map['CurrencyInput'] = $order->commerce_order_total['und'][0]['currency_code'];
	$request_map['TxnData1'] = '';
	$request_map['TxnData2'] = '';
	$request_map['TxnData3'] = '';
	$request_map['Opt'] = 'TO=1212062221';
	$request_map['EmailAddress'] = $order->mail;
	$request_map['TxnId'] = $transaction->transaction_id;
	$request_map['UrlSuccess'] = url('checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key'], array('absolute' => TRUE));
	$request_map['UrlFail'] = url('checkout/' . $order->order_id . '/payment/back/' . $order->data['payment_redirect_key'], array('absolute' => TRUE));

	return $request_map;

}


function _commerce_oz_pxpay_post_request($request_map) {
		
		$request_xml = _commerce_oz_array2xml($request_map, '<GenerateRequest/>');
		$request_xml = str_replace('<?xml version="1.0"?>', '', $request_xml);
		
		dsm($request_xml);
				
		$URL = "https://sec.paymentexpress.com/pxpay/pxaccess.aspx";
		              
		$ch = curl_init(); 
		curl_setopt($ch, CURLOPT_URL,  $URL);
		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_POSTFIELDS, $request_xml);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0); //Needs to be included if no *.crt is available to verify SSL certificates
		curl_setopt($ch, CURLOPT_SSLVERSION, 3);  
		$result = curl_exec ($ch); 
		curl_close ($ch);
		
		echo $result;
		
		dsm($result, 'raw result');
		
		return json_decode(json_encode((array) simplexml_load_string($result)),1);;
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


function _commerce_oz_paymentexpress_3p_close_transaction($transaction,  $request_map, $response_map) {
	
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







