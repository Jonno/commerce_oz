<?php

/**
 * @file
 * Implements PaymentExpress PXPay (Server Hosted) for use in Drupal Commerce.
 */

//define(COMMERCE_OZ_POLI_XML_SCHEMA, ' xmlns="http://schemas.datacontract.org/2004/07/Centricom.POLi.Services.MerchantAPI.Contracts" xmlns:i="http://www.w3.org/2001/XMLSchema-instance">');


/**
 * Payment method callback: checkout form.
 * loading the default form from Commerce Payment module
 */
function commerce_oz_poli_redirect_form($form, &$form_state, $order, $payment_method) {

  
  // Transaction record created for the transaction id
  $transaction = _commerce_oz_open_transaction($payment_method['base'], $order->order_id);
 
  
  
  
 // $bank_list = _commerce_oz_poli_get_bank_list();
  
 // dsm($bank_list);
  
 	$request_xml = _commerce_oz_poli_build_request_2($transaction, $payment_method, $order, 'iBankAU01');
  
   	$request_string = str_replace('<a:', '<', $request_xml);
		$request_string = str_replace('</a:', '</', $request_string);
  	
  	$request_map = json_decode(json_encode((array) simplexml_load_string($request_string)), 1);

  
  
 // watchdog('commoz', nl2br(print_r($request_xml, true)));
  
 // dsm($request_map, '$request_map');
	
	 $response_map = _commerce_oz_poli_post_request($request_xml);
	
	 
	 watchdog('commoz', nl2br(print_r($response_map, true)));
	
	
	
	$form['#action'] = $response_map['Transaction']['NavigateURL'];
	
  $form['poli_head']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Proceed to POLi Payments'),
  );

  return $form;
  
}

function commerce_oz_poli_redirect_form_validate($order, $payment_method) {

return TRUE;

}


function _commerce_oz_poli_post_request($request_xml) {
		

	//	$request_xml = _commerce_oz_array2xml($request_map, '<poli/>');
		
	// watchdog('commoz', $request_xml);

  	$URL = "https://merchantapi.apac.paywithpoli.com/MerchantAPIService.svc/Xml/transaction/initiate";
		              
		$ch = curl_init(); 
		curl_setopt($ch, CURLOPT_URL,  $URL);
		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-type: text/xml'));
		curl_setopt($ch, CURLOPT_POSTFIELDS, $request_xml);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_FORBID_REUSE, 1);
    curl_setopt($ch, CURLOPT_FRESH_CONNECT, 1);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0); 		
		curl_setopt($ch, CURLOPT_SSLVERSION,3);  
		$result = curl_exec ($ch); 
		curl_close ($ch);
  	
  //	dsm($$result, '$result');
  	
  //	return ;
  	
  	$result = str_replace('<a:', '<', $result);
		$result = str_replace('</a:', '</', $result);
  	
  	$response_map = json_decode(json_encode((array) simplexml_load_string($result)), 1);

		return $response_map;
  	

}






function _commerce_oz_poli_build_request($transaction, $payment_method, $order, $fi_code) {

	$settings = _commweb_get_var('commerce_oz_gateway_poli');
	
	//Generate the MerchantTxnRef and the OrderInfo
	$merchTxnRef = $order->order_id . '-' . $transaction->transaction_id;
	$redirect_key = ''; // $order->data['payment_redirect_key'];
	$request_map = array();
	
	$request_map['MerchantCode'] = $settings['poli_merchant']; 
	$request_map['AuthenticationCode'] = $settings['poli_auth_code'];
	$request_map['MerchantRef'] = $merchTxnRef;	
	$request_map['CurrencyAmount'] = sprintf("%01.2f", ($order->commerce_order_total['und'][0]['amount'])/100);
	$request_map['CurrencyCode'] = $order->commerce_order_total['und'][0]['currency_code'];
	$request_map['SelectedFICode'] = $fi_code;
	$request_map['MerchantDateTime'] = date('Y-m-d\TH:i:s');
	$request_map['Timeout'] = 3600; // 1 hour

	$request_map['NotificationURL'] = url('checkout/' . $order->order_id . '/payment/return/' . $redirect_key, array('absolute' => TRUE));
		
	$request_map['SuccessfulURL'] = url('checkout/' . $order->order_id . '/payment/return/' . $redirect_key, array('absolute' => TRUE));
	
	$request_map['UnsuccessfulURL'] = url('checkout/' . $order->order_id . '/payment/back/' . $redirect_key, array('absolute' => TRUE));

	$request_map['MerchantCheckoutURL'] = url('checkout/' . $order->order_id . '/payment/back/' . $redirect_key, array('absolute' => TRUE));
	
	$request_map['MerchantHomePageURL'] = url('', array('absolute' => TRUE));
	
	return $request_map;

}

function _commerce_oz_poli_build_request_2($transaction, $payment_method, $order, $fi_code) {

	$settings = _commweb_get_var('commerce_oz_gateway_poli');
	
	
	$redirect_key = $order->data['payment_redirect_key'];
	
	//Generate the MerchantTxnRef and the OrderInfo
	$merchTxnRef = $order->order_id . '-' . $transaction->transaction_id;


	$xml = '<InitiateTransactionRequest xmlns="http://schemas.datacontract.org/2004/07/Centricom.POLi.Services.MerchantAPI.Contracts" xmlns:i="http://www.w3.org/2001/XMLSchema-instance">
<AuthenticationCode>' . $settings['poli_auth_code'] . '</AuthenticationCode>
<Transaction xmlns:dco="http://schemas.datacontract.org/2004/07/Centricom.POLi.Services.MerchantAPI.DCO">
<dco:CurrencyAmount>' . sprintf("%01.2f", ($order->commerce_order_total['und'][0]['amount'])/100) . '</dco:CurrencyAmount>
<dco:CurrencyCode>' . 'AUD' . '</dco:CurrencyCode>
<dco:MerchantCheckoutURL>' . url('checkout/' . $order->order_id . '/payment/back/' . $redirect_key, array('absolute' => TRUE)) . '</dco:MerchantCheckoutURL>
<dco:MerchantCode>' . $settings['poli_merchant'] . '</dco:MerchantCode>
<dco:MerchantData>' . $merchTxnRef . '</dco:MerchantData>
<dco:MerchantDateTime>' . date('Y-m-d\TH:i:s') . '</dco:MerchantDateTime>
<dco:MerchantHomePageURL>' . url('', array('absolute' => TRUE)) . '</dco:MerchantHomePageURL>
<dco:MerchantRef>' . $merchTxnRef . '</dco:MerchantRef>
<dco:SelectedFICode i:nil="true" />
<dco:SuccessfulURL>' . url('checkout/' . $order->order_id . '/payment/return/' . $redirect_key, array('absolute' => TRUE)) . '</dco:SuccessfulURL>
<dco:Timeout>1000</dco:Timeout>
<dco:UnsuccessfulURL>' . url('checkout/' . $order->order_id . '/payment/back/' . $redirect_key, array('absolute' => TRUE)) . '</dco:UnsuccessfulURL>
<dco:UserIPAddress>' . '127.0.0.1' . '</dco:UserIPAddress>
</Transaction>
</InitiateTransactionRequest>';

return $xml;

}












function _commerce_oz_poli_close_transaction($transaction,  $request_map, $response_map) {
	
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


/* 	_commerce_oz_poli_get_bank_list() 
 *	returns an array of suitable financial institutions for POLi payments
 *
 */

function _commerce_oz_poli_get_bank_list() {
		
		$settings = _commweb_get_var('commerce_oz_gateway_poli');
		
	  $xml = '<GetFinancialInstitutionsRequest xmlns="http://schemas.datacontract.org/2004/07/Centricom.POLi.Services.MerchantAPI.Contracts" xmlns:i="http://www.w3.org/2001/XMLSchema-instance">';
	  $xml .= '<AuthenticationCode>' . $settings['poli_auth_code'] . '</AuthenticationCode>';
	  $xml .= '<MerchantCode>' . $settings['poli_merchant'] . '</MerchantCode>';
	  $xml .= '</GetFinancialInstitutionsRequest>';

  	$URL = "https://merchantapi.apac.paywithpoli.com/MerchantAPIService.svc/Xml/banks";
		              
		$ch = curl_init(); 
		curl_setopt($ch, CURLOPT_URL,  $URL);
		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-type: text/xml'));
		curl_setopt($ch, CURLOPT_POSTFIELDS, $xml);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_FORBID_REUSE, 1);
    curl_setopt($ch, CURLOPT_FRESH_CONNECT, 1);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0); 		
		curl_setopt($ch, CURLOPT_SSLVERSION,3);  
		$result = curl_exec ($ch); 
		curl_close ($ch);
  
  	$result = str_replace('<a:', '<', $result);
		$result = str_replace('</a:', '</', $result);
  	
  	$bank_list = json_decode(json_encode((array) simplexml_load_string($result)), 1);

  	
  	if(sizeof($bank_list['Errors']) == '0') {
  		
  		$banks = array();
  		
  		foreach($bank_list['FinancialInstitutionList'] as $institution) {
  			$banks[$institution['FinancialInstitutionCode']] = $institution['FinancialInstitutionName'];
  		}
  		
  		return $banks;
  		
  	} else {
  		return FALSE;
  	}
}

