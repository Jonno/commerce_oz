<?php

/**
 * @file
 * Implements CommWeb Virtual Payment Service for use in Drupal Commerce.
 */

function commerce_oz_refund_build_migs_refund_payload($merchTxnRef, $amount, $trans_no) 

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


function commerce_oz_refund_post_migs_refund_request($post_data)
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



function commerce_oz_refund_process_migs_refund_request($request_string)
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


function commerce_oz_refund_process_vpc_refund_response($transaction, $response_string, $request_map, $order_id = 0, $old_trans_id = 0, $test_message = '')
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





