<?php

/**
 * @file
 * Implements Commerce Oz Refunds for use in Drupal Commerce.
 */



/**
 *
 * @param $order
 *   An order id.
 * @param $transaction
 *   A transaction id.
 */
function commerce_oz_refund_transaction($order, $transaction) {
  
	// 	dsm($order, '$order');
	// 	dsm($transaction, '$transaction');
		
	//	$pm = commerce_oz_load_gateway_payment_methods('cba');
	//	dsm($pm);
		
		
		
		$method_id = $transaction->payment_method;
		$arr = explode('|', $method_id);
		$arr = explode('_', $arr[0]);
		
		$gateway_key = $arr[0];
	//	dsm($gateway_key, '$$gateway_key');

		$gateway = commerce_oz_load_payment_method_gateway($transaction->payment_method);
		
	//	dsm($gateway, '$gateway');
		
		$found = module_load_include('inc',  'commerce_oz',  $gateway['file']);
		
		
		if($gateway) {
			call_user_func_array($gateway['callbacks']['refund'], array($order, $transaction, $gateway));
		}
		
		drupal_goto('admin/commerce/orders/' . $order->order_id . '/payment');
	
		return;
}	


function commerce_oz_refund_transaction_old($order, $transaction) 
{
//	// dsm($order, 'Order');
//	// dsm($transaction, 'Transaction');
	commerce_oz_refund_transaction_action($transaction, $context) ;
	drupal_goto('admin/commerce/orders/' . $order->order_id . '/payment');
}


function commerce_oz_refund_transaction_action_form($context) {

	// // dsm($context);

	$form = array();
  
   /* Provide a textfield of to pass different Amounts to refund  */
 $form['amount'] = array(
     '#input' => TRUE,

    '#type' => 'textfield',
    '#size' => 6,
    '#maxlength' => 10,
   		
      '#title' => t('Refund Amount'), 
      '#default_value' => $order->commerce_order_total['und'][0]['amount'],
      '#disabled'  => FALSE,
      '#description' => t('Update the Amount (in cents) to refund.'),
      '#required' => FALSE, 
    );  

  
  return $form;
  
  }



/**
 * Validates settings form for action().
 * Verifies that user exists before continuing.
 */
function commerce_oz_refund_transaction_action_validate($form, $form_state) {
  
  // No validation
}

/**
 * Submit handler for action_example_node_sticky_action.
 *
 * Returns an associative array of values which will be available in the
 * $context when an action is executed.
 */
function commerce_oz_refund_transaction_action_submit($form, $form_state) {
  return array('lawnsale' => $form_state['values']['lawnsale']);
}

	function parseResponse($xmlResponse){
		$xml_parser = xml_parser_create();
		xml_parse_into_struct($xml_parser,  $xmlResponse, $xmlData, $index);
        $responseFields = array();
        foreach($xmlData as $data)
            if(isset($data["level"]))
	    	if($data["level"] == 2)
        		$responseFields[$data["tag"]] = isset($data["value"]) ? $data["value"]:"";
        return $responseFields;
	}




/**
 *
 * @param $node
 *   A node object provided by the associated trigger.
 * @param $context
 *   Array with the following elements:
 *   - 'author': username of the author's content this function will promote and
 *     set as sticky.
 */
function commerce_oz_refund_transaction_action($node, $context) 
{
  
	// dsm($node, 'Transaction $node');

	
	if(substr($node->payment_method, 'eway')) {
	
		$eway_settings = _commweb_get_var('commerce_oz_gateway_eway');
		
		$payload = $node->payload;
		
		$xmlRequest = '<ewaygateway><ewayCustomerID>' . $eway_settings['eway_customer_id'] . '</ewayCustomerID>'; 
		$xmlRequest .= '<ewayOriginalTrxnNumber>' . $node->remote_id . '</ewayOriginalTrxnNumber>'; 
		$xmlRequest .= '<ewayTotalAmount>' . $payload['response']['ewayTotalAmount'] . '</ewayTotalAmount>'; 
		$xmlRequest .= '<ewayCardExpiryMonth>' . $payload['request']['ewayCardExpiryMonth'] . '</ewayCardExpiryMonth>'; 
		$xmlRequest .= '<ewayCardExpiryYear>' . $payload['request']['ewayCardExpiryYear'] . '</ewayCardExpiryYear>'; 
		$xmlRequest .= '<ewayOption1></ewayOption1><ewayOption2></ewayOption2><ewayOption3></ewayOption3>'; 
		$xmlRequest .= '<ewayRefundPassword>eightCap9</ewayRefundPassword></ewaygateway>';

		$ch = curl_init('https://www.eway.com.au/gateway/xmltest/refund_test.asp');
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $xmlRequest);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		$xmlResponse = curl_exec($ch);
		
		// dsm($xmlRequest, '$xmlRequest');
		// dsm($xmlResponse, '$xmlResponse');
		
		$response_array = parseResponse($xmlResponse);
			// dsm($response_array, '$response_array');
		
		if(curl_errno( $ch ) == CURLE_OK) {
		
			$response_array = parseResponse($xmlResponse);
			// dsm($response_array, '$response_array');
		}
			
	}	
		//	rules_invoke_event('commerce_oz_refund_complete', $transaction);
		
			
	// Write a Transaction record
	$transaction = commerce_payment_transaction_new($node->payment_method, $node->order_id);
	
	// Save the transaction information.
	commerce_payment_transaction_save($transaction);	
		
		
		
}		
		/*
		
		$merchTxnRef = $node->payload['request']['vpc_MerchTxnRef'];
		$amount = $node->payload['request']['vpc_Amount'];
		$trans_no = $node->payload['response']['vpc_TransactionNo'];
		$order_id = $node->order_id;
		
		$refund_request_string = _build_vpc_refund_payload($merchTxnRef, $amount, $trans_no);
		$request_map = _process_vpc_refund_request($refund_request_string);
		
		$response_string = _post_vpc_refund_request($refund_request_string);
		$response_map = _process_vpc_refund_response($transaction, $response_string, $request_map, $order_id, $node->transaction_id);
		
	//	// dsm($node, 'Transaction $node');


		
		// Save the transaction information.
		commerce_payment_transaction_save($transaction);
		
		_update_source_transaction($node, $transaction);
			*/
			
				



function _build_vpc_refund_payload($merchTxnRef, $amount, $trans_no) 

{
		
		// Get the API Credentials
		
		$settings = _commweb_get_var('commerce_oz_gateway_cba');
		
		$merchantid = $settings['merchant_id'];
		$accesscode = $settings['access_code'];
		$user = $settings['refund_username'];
		$password = $settings['refund_password'];
		
		$postData = '';
		$amp = '&';
		
		$postData .= 'vpc_Version' . '=' . COMMERCE_COMMWEB_vpc_Version . $amp;
		$postData .= 'vpc_Command' . '=' . 'refund' . $amp;
		$postData .= 'vpc_MerchTxnRef' . '=' . $merchTxnRef . $amp;
		$postData .= 'vpc_AccessCode' . '=' . $accesscode . $amp;
		$postData .= 'vpc_Merchant' . '=' . $merchantid . $amp; 
		$postData .= 'vpc_TransNo' . '=' . $trans_no . $amp;
		$postData .= 'vpc_Amount' . '=' . $amount . $amp;
		$postData .= 'vpc_User' . '=' . $user . $amp;
		$postData .= 'vpc_Password' . '=' . $password;
		
		return $postData;
}


function _post_vpc_refund_request($post_data)
{
	ob_start();
	
	// initialise
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



function _process_vpc_refund_request($request_string)
{
		
		module_load_include('inc', 'commerce_oz', 'commerce_oz_migs_2p');
	  $request_map = array();
		parse_str($request_string, $request_map);
		$request_map['vpc_Merchant'] = _commweb_build_merchant_string($request_map['vpc_Merchant']);
		$request_map['vpc_AccessCode'] = _commweb_build_accesscode_string($request_map['vpc_AccessCode']);
		$request_map['vpc_Password'] = _commweb_build_refund_user_password_string($request_map['vpc_Password']);
		$request_map['vpc_User'] = _commweb_build_refund_user_string($request_map['vpc_User']);
		return $request_map;
}


function _process_vpc_refund_response($transaction, $response_string, $request_map, $order_id = 0, $old_trans_id = 0, $test_message = '')
{
	// Map VPC repsonse to array
	$response_map = array();
	parse_str($response_string, $response_map);
	$response_map['vpc_Merchant'] = _commweb_build_merchant_string($response_map['vpc_Merchant']);
	
	// Update the transaction object
  $transaction->instance_id = 'commerce_commweb|commerce_payment_commerce_commweb';
  $transaction->amount = $response_map['vpc_Amount'];
  $transaction->currency_code = 'AUD';
  $transaction->payload = array('request' => $request_map, 
  															'response' => $response_map
  															);
  
  $transaction->remote_id = $response_map['vpc_TransactionNo'];    
  $transaction->remote_status = $response_map['vpc_TxnResponseCode'];  
  
  $transaction->status = $response_map['vpc_TxnResponseCode'] ? COMMERCE_PAYMENT_STATUS_FAILURE : COMMERCE_PAYMENT_STATUS_SUCCESS;
  
  $transaction->message = $test_message . '<strong>Refund</strong> Code: @code - @message <br />Order ID: <a href="/admin/commerce/orders/@order_id">@order_id</a> <br />Refunded Transaction: <a href="/admin/commerce/orders/@order_id/payment/@trans_id/view">@trans_id</a>'; 
  
  $transaction->message_variables = array(
  	'@code' => $response_map['vpc_TxnResponseCode'],
  	'@message' => $response_map['vpc_Message'],
  	'@trans_id' => $old_trans_id,
  	'@order_id' => $order_id,
  		);
  
  return $response_map;		
  		
}

function _update_source_transaction($transaction, $refund_transaction)
{
	
	// Update the source transaction object message
  
  $transaction->message = $transaction->message . '<br />Refunded Code: @code - @message 
  <br />Refund Transaction: <b>@refund_trans_id</b>'; 
  
  $transaction->message_variables = $transaction->message_variables + array(
  	'@refund_trans_id' => $refund_transaction->transaction_id,
  	'@order_id' => $transaction->order_id,
  		);
  
    $transaction->payload = array('request' => $transaction->payload['request'], 
  															'response' => $transaction->payload['response'],
  															'refunded' => $refund_transaction->payload['response'],
  															);
  
  commerce_payment_transaction_save($transaction);
  
 // // dsm($transaction, 'Source Transaction');
 // // dsm($refund_transaction, 'Refund Transaction');
  		
}

function commerce_oz_refund_update_source_transaction($transaction, $refund_transaction)
{
	
	// Update the source transaction object message
  
  $transaction->message = $transaction->message . '<br />Refunded Code: @code - @message 
  <br />Refund Transaction: <b>@refund_trans_id</b>'; 
  
  $transaction->message_variables = $transaction->message_variables + array(
  	'@refund_trans_id' => $refund_transaction->transaction_id,
  	'@order_id' => $transaction->order_id,
  		);
  
    $transaction->payload = array('request' => $transaction->payload['request'], 
  															'response' => $transaction->payload['response'],
  															'refunded' => $refund_transaction->payload['response'],
  															);
  
  commerce_payment_transaction_save($transaction);
  
 // // dsm($transaction, 'Source Transaction');
 // // dsm($refund_transaction, 'Refund Transaction');
  		
}


