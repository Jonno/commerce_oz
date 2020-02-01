<?php

/**
 * @file
 * Implements PaymentExpress PXPay (Server Hosted) for use in Drupal Commerce.
 */

define(COMMERCE_OZ_POLI_XML_SCHEMA, ' xmlns="http://schemas.datacontract.org/2004/07/Centricom.POLi.Services.MerchantAPI.Contracts" xmlns:i="http://www.w3.org/2001/XMLSchema-instance">');


/**
 * Payment method callback: checkout form.
 * loading the default form from Commerce Payment module
 */
function commerce_oz_poli_redirect_form($form, &$form_state, $order, $payment_method) {

		$logopath = '/' . drupal_get_path('module', 'commerce_oz') . '/image/poli_68.png';
	
	$desc_text = '<a href="http://www.polipayments.com"><img src="' . $logopath . '" /></a><br />The <a href="http://www.polipayments.com/">POLi Payments</a> gateway provides <a href="http://www.polipayments.com/consumer">a unique service for online purchases without a credit card</a> by redirecting you to your online banking service to effect a bank transfer to this seller\'s account.<br />Once you have completed the transfer you will be redirected back to this site to collect your receipt.';

	$more_info_text = 'The POLi Payments system is very simple. <ul><li>Click the Proceed button to select your bank<li>Fill in the Transfer Funds page in your online banking console<li>Click the Save button in the POLi header<li>The transfer is processed and you are returned to this site</ul>';


  $form['poli_head'] = array(
    '#type' => 'fieldset',
    '#title' => $payment_method['title'],
    '#collapsible' => FALSE, 
  	'#collapsed' => FALSE,
    '#description' => t($desc_text),
  );
  
   $form['poli_head']['more_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('More Information'),
    '#collapsible' => TRUE, 
  	'#collapsed' => TRUE,
    '#description' => t($more_info_text),
  );
  
  
  
  
  
  
  $bank_list = _commerce_oz_poli_get_bank_list();
  
  dsm($bank_list);
  
  $request_map = _commerce_oz_poli_build_request($transaction, $payment_method, $order, $bank_list[0]);
  
  dsm($request_map, '$request_map');
	
	 _commerce_oz_poli_post_request($request_map);
	
	  $form['poli_head']['select_bank'] = array(
    '#type' => 'select',
    '#title' => t('Select your bank from the lists below'),
    '#options' => $bank_list,
    '#description' => t('You will transferred to your bank\'s internet banking service to make a funds transfer.'),
    '#default' => 'error',
  );
	

	
  $form['poli_head']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Proceed to POLi Payments'),
  );

  return $form;
  
}

function _commerce_oz_poli_post_request($request_map) {
		

		$request_xml = _commerce_oz_array2xml($request_map, '<ematters />');
		
		dsm($request_xml, '$request_xml');

  	$URL = "https://merchantapi.apac.paywithpoli.com/MerchantAPIService.svc/Xml/transaction/initiate";
		              
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
  	
  	dsm($$result, '$result');
  	
  	return ;
  	
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









/**
 * Payment method callback: checkout form validation.
 */
function commerce_oz_poli_redirect_form_validate($order, $payment_method) {

return false;

}

/*

function poli_test() {

	
	$settings = _commweb_get_var('commerce_oz_gateway_poli');
	
	$xml  = '<GetTransactionRequest ' . COMMERCE_OZ_POLI_XML_SCHEMA;
	$xml .= '<AuthenticationCode>' . $settings['poli_auth_code'] . '</AuthenticationCode>';
	$xml .= '<MerchantCode>' . $settings['poli_merchant'] . '</MerchantCode>';
	$xml .= '</GetTransactionRequest>';



	$url='https://merchantapi.apac.paywithpoli.com/MerchantAPIService.svc/Xml/transaction/query';
	
	
	$response=$this->sendTransactionToGateway($url, $xml);
	$response=str_replace('<a:','<',$response);$response=str_replace('</a:','</',$response);
	$result = @simplexml_load_string($response);

	if (empty($response) || !isset($result->TransactionStatusCode)) tep_redirect(tep_href_link(FILENAME_CHECKOUT_PAYMENT, 'payment_error=' . $this->code . '&error='.urlencode(MODULE_PAYMENT_POLI_ERROR_GENERAL), 'SSL'));
	if (isset($result->Errors->Error->Message)) tep_redirect(tep_href_link(FILENAME_CHECKOUT_PAYMENT, 'payment_error=' . $this->code . '&error='.urlencode($result->Errors->Error->Message), 'SSL'));

	$amount=$this->format_raw($order->info['total'],MODULE_PAYMENT_POLI_CURRENCY);
	$amountPaid=$result->Transaction->AmountPaid;
	$PaymentAmount=$result->Transaction->PaymentAmount;
	if ($result->TransactionStatusCode != "Completed") tep_redirect(tep_href_link(FILENAME_CHECKOUT_PAYMENT, 'payment_error=' . $this->code . '&error='.urlencode(MODULE_PAYMENT_POLI_ERROR_GENERAL), 'SSL'));

	if ($amount != $amountPaid || $amountPaid != $PaymentAmount) tep_redirect(tep_href_link(FILENAME_CHECKOUT_PAYMENT, 'payment_error=' . $this->code . '&error='.urlencode(MODULE_PAYMENT_POLI_ERROR_GENERAL), 'SSL'));

	return true;
	
      } else {
      
	$xml = '';
	
	$xml .= '<InitiateTransactionRequest xmlns="http://schemas.datacontract.org/2004/07/Centricom.POLi.Services.MerchantAPI.Contracts" xmlns:i="http://www.w3.org/2001/XMLSchema-instance">
<AuthenticationCode>'.MODULE_PAYMENT_POLI_AUTHENTICATION_CODE.'</AuthenticationCode>
<Transaction xmlns:dco="http://schemas.datacontract.org/2004/07/Centricom.POLi.Services.MerchantAPI.DCO">

<dco:CurrencyAmount>'.$this->format_raw($order->info['total'],MODULE_PAYMENT_POLI_CURRENCY).'</dco:CurrencyAmount>

<dco:CurrencyCode>'.MODULE_PAYMENT_POLI_CURRENCY.'</dco:CurrencyCode>

<dco:MerchantCheckoutURL>'.tep_href_link(FILENAME_CHECKOUT_PAYMENT, '', 'SSL').'</dco:MerchantCheckoutURL>

<dco:MerchantCode>'.MODULE_PAYMENT_POLI_MERCHANT_CODE.'</dco:MerchantCode>

<dco:MerchantData>'.substr(date('YmdHis') . '-' . $customer_id . '-' . $cartID,0,1000).'</dco:MerchantData>

<dco:MerchantDateTime>'.date('Y-m-d\TH:i:s').'</dco:MerchantDateTime>

<dco:MerchantHomePageURL>'.tep_href_link(FILENAME_DEFAULT).'</dco:MerchantHomePageURL>

<dco:MerchantRef>'.substr(STORE_NAME, 0, 100).'</dco:MerchantRef>

<dco:SelectedFICode i:nil="true" />

<dco:SuccessfulURL>'.tep_href_link(FILENAME_CHECKOUT_PROCESS, 'check=SERVER&' . tep_session_name() . '=' . tep_session_id(), 'SSL', false).'</dco:SuccessfulURL>

<dco:Timeout>1000</dco:Timeout>

<dco:UnsuccessfulURL>'.tep_href_link(FILENAME_CHECKOUT_PAYMENT, 'payment_error=' . $this->code . '&error='.urlencode(MODULE_PAYMENT_POLI_ERROR_GENERAL), 'SSL').'</dco:UnsuccessfulURL>

<dco:UserIPAddress>'.tep_get_ip_address().'</dco:UserIPAddress>

</Transaction>

</InitiateTransactionRequest>';


	$url='https://merchantapi.apac.paywithpoli.com/MerchantAPIService.svc/Xml/transaction/initiate';
	
	
	$response=$this->sendTransactionToGateway($url, $xml);
	$response=str_replace('<a:','<',$response);$response=str_replace('</a:','</',$response);
	$result = @simplexml_load_string($response);
//print_r($result);exit;
//print $xml;exit;







}


*/











/**
 * Payment method callback: checkout form submission.
 *
 * 
 */
function commerce_oz_poli_redirect_form_submit($order, $payment_method) {

 
 
}




/**
 * Payment method callback: settings form.
 */
function commerce_oz_poli_settings_form($settings = NULL) {

  $form = array();
  

  return $form;
}


function _commerce_oz_poli_build_request($transaction, $payment_method, $order, $fi_code) {

	$settings = _commweb_get_var('commerce_oz_gateway_paymentexpress');
	
	//Generate the MerchantTxnRef and the OrderInfo
	$merchTxnRef = $order->order_id . '-' . $transaction->transaction_id;
	$redirect_key = $order->data['payment_redirect_key'];
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
	
	$request_map['MerchantHomePageURL'] = url('/', array('absolute' => TRUE));



	return $request_map;

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
		
	  $xml = '<GetFinancialInstitutionsRequest ' . COMMERCE_OZ_POLI_XML_SCHEMA;
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

