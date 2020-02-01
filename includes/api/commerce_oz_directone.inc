<?php

/**
 * Payment method callback: payment transfer form.
 */
function commerce_oz_directone_redirect_form($form, &$form_state, $order, $payment_method) {

	$logopath = '/' . drupal_get_path('module', 'commerce_oz') . '/image/' . $payment_method['gateway_provider'] . '_68.png';
	
	$descText = '<br /><a href="http://www.eway.com.au"><img src="' . $logopath . '" /></a><br /><br />Click the \'Proceed to eWayz\' button to complete payment of your purchase at eWay.<br />On completing your transaction, you will be taken to your Receipt information.<br />';

  $form['commerce_oz_3p_head'] = array(
    '#type' => 'fieldset',
    '#title' => $payment_method['title'],
    '#collapsible' => FALSE, 
  	'#collapsed' => FALSE,
    '#description' => t($descText),
  );

	// Transaction record
  $transaction =  _commerce_oz_open_transaction($payment_method['method_id'], $order->order_id);

	$wrapper = entity_metadata_wrapper('commerce_order', $order);

  // Get financial info
  $currency_code = $wrapper->commerce_order_total->currency_code->value();
  $amount = $wrapper->commerce_order_total->amount->value();
	
	$request_map = _commerce_oz_directone_build_purchase_request($payment_method, $order, $transaction, $amount);
	
//	dsm($request_map, '$request_map');
//	dsm($order, '$order');

	
	//$responseurl = $request_map['vpc_ReturnURL'];

//	_commerce_oz_directone_post_request($request_map);
	
	$form['#action'] = 'https://vault.safepay.com.au/cgi-bin/test_payment.pl';

  $form['commerce_oz_3p_head']['vendor_name'] = array(
    '#type' => 'hidden',
    '#value' => 'psycadia',
  );


	foreach($order->commerce_line_items['und'] as $key => $value ) {
		
		$lineitem = commerce_line_item_load($value);
	//	dsm($lineitem, '$lineitem');
		
		$product = commerce_product_load($lineitem->commerce_product['und'][0]);
	//	dsm($product, '$product');
		
		$form['commerce_oz_3p_head'][$product->title] = array(
    		'#type' => 'hidden',
    		'#value' => $lineitem->quantity . ',' . $product->commerce_price['und'][0]['amount']/100,
  );

	}



$form['commerce_oz_3p_head']['hidden_fields'] = array(
    '#type' => 'hidden',
    '#value' => 'op,form_id,form_token,form_build_id',
  );




 $form['commerce_oz_3p_head']['reply_link_url'] = array(
    '#type' => 'hidden',
    '#value' => url('checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key'], array('absolute' => TRUE)),
  );

 $form['commerce_oz_3p_head']['return_link_url'] = array(
    '#type' => 'hidden',
    '#value' => url('checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key'], array('absolute' => TRUE)),
  );
	
  $form['commerce_oz_3p_head']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Proceed to Direct One'),
  );
	
//	drupal_goto($poststring);
	
	return $form;
	
}



function commerce_oz_directone_redirect_form_validate($order, $payment_method)
{

  dsm($_GET);
  
      	watchdog(	'commerce_oz',  
    						'Request  Data  was:  @requestdata',  
    						array('@requestdata'  =>  print_r($_GET,  true)),    
    						WATCHDOG_DEBUG);
   
}

/**
 * Payment method callback: payment transfer form.
 * Could be shown on Review Screen?
 */
function commerce_oz_directone_payment_submit_form($payment_method, $pane_values, $checkout_pane, $order) {

  $form = array();
	
	$gateway_provider = commerce_oz_load_gateway_provider($payment_method['gateway_provider']);
	
	$logopath = '/' . drupal_get_path('module', 'commerce_commweb') . '/image/' . $gateway_provider['base']  . '_68.png';
	
	$descText = '<img src="' . $logopath . '" /><br />Click the Pay Now button to be transferred to the ' .  $gateway_provider['owner'] . ' Payment Servers.<br />On completing your transaction, you will be returned to this page for your Receipt information.<br />';

  $form['commerce_oz_payment_transfer'] = array(
    '#type' => 'fieldset',
    '#title' => t($payment_method['title']),
    '#collapsible' => FALSE, 
  	'#collapsed' => FALSE,
    '#description' => t($descText),
  );
	
	
  $form['commerce_oz_payment_transfer']['pay_now_button'] = array(
    '#type' => 'button',
    '#prefix' => '<br />',
    '#value' => t('Pay Now'), 
   
  );
	
  return $form;
}



function _commerce_oz_directone_build_purchase_request($payment_method, $order, $transaction, $amount)
{
		// Get the API Credentials
		
		$settings = _commweb_get_var('commerce_oz_gateway_' . $payment_method['gateway_provider']);
		
	// Make the request_map array
		$request_map = array(
			'vendor_name' => 	'psycadia', //	$settings['directone_username'],
		);


		return $request_map;	
}


function _commerce_oz_directone_post_request($request_map) {
		
				
		$URL = "https://vault.safepay.com.au/cgi-bin/test_payment.pl";
		              
		$ch = curl_init(); 
		curl_setopt($ch, CURLOPT_URL,  $URL);
		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_POSTFIELDS, $request_map);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0); 		
		curl_setopt($ch, CURLOPT_SSLVERSION,3);  
		$result = curl_exec ($ch); 
		curl_close ($ch);
		
		dsm($result);
		
		
		return ; //json_decode(json_encode((array) simplexml_load_string($result)),1);;
}









function _commerce_oz_directone_close_transaction($transaction,  $request_map, $response_map) {
	
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





