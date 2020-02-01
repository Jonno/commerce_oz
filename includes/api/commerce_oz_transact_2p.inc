<?php

/**
 * @file
 * Implements NAB payment services for use in Drupal Commerce.
 */

define('COMMERCE_NAB_TRANSACT_AUTH_CAPTURE', 0);
define('COMMERCE_NAB_TRANSACT_AUTH_ONLY', 11);

/**
 * Determines access to the prior authorization capture form for NAB
 *   credit card transactions.
 *
 * @param $order
 *   The order the transaction is on.
 * @param $transaction
 *   The payment transaction object to be captured.
 *
 * @return
 *   TRUE or FALSE indicating capture access.
 */
function commerce_nab_transact_capture_access($order, $transaction) {
  // Return FALSE if the transaction isn't for NAB or isn't
  // awaiting capture.
  if ($transaction->payment_method != 'nab_transact' || strtoupper($transaction->remote_status) != 'AUTH_ONLY') {
    return FALSE;
  }

  // Return FALSE if it is more than 30 days past the original authorization.
  if (time() - $transaction->created > 86400 * 30) {
    return FALSE;
  }

  // Allow access if the user can update payments on this order.
  return commerce_payment_transaction_access('update', $order, $transaction);
}

/**
 * Implements hook_commerce_payment_method_info().
 */
function commerce_nab_transact_commerce_payment_method_info() {
  $payment_methods = array();

  $payment_methods['nab_transact'] = array(
    'base' => 'commerce_nab_transact',
    'title' => t('NAB Transact'),
    'short_title' => t('NAB Transact'),
    'display_title' => t('Credit card via NAB gateway'),
    'description' => t('Adds functionality to process transactions with the NAB API'),
  );

  return $payment_methods;
}

/**
 * Payment method callback: settings form.
 */
function commerce_oz_transact_settings_form($settings = NULL) {
  $form = array();
  
  // Add some defaults.
  $settings = (array)$settings + array(
    'commerce_nab_currency' => 'AUD'
  );

  $form['commerce_nab_currency'] = array(
    '#type' => 'textfield',
    '#title' => t('Currency code'),
    '#description' => t('Defaults to AUD; support for other currencies is not known.'),
    '#default_value' => $settings['commerce_nab_currency'],
  );
  $form['commerce_nab_transact_po_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('PO prefix'),
    '#description' => t('Enter a string to place before the order ID when specifying a PO number through NAB Transact.'),
    '#default_value' => $settings['commerce_nab_transact_po_prefix'],
  );

  $form['commerce_nab_transact_xml_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('XML API settings'),
    '#description' => t('This module currently only integrates the XML API provided by NAB Transact for processing credit cards.  You must fill in the appropriate information below and ensure you have SSL enabled for the site.'),
  );
  $form['commerce_nab_transact_xml_settings']['commerce_nab_xml_merchant_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant ID'),
    '#description' => t('The 7 character merchant ID for Credit Card transactions, eg: "ABC0001".'),
    '#default_value' => $settings['commerce_nab_transact_xml_settings']['commerce_nab_xml_merchant_id'],
  );
  $form['commerce_nab_transact_xml_settings']['commerce_nab_xml_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#description' => t('Your password for making XML API requests.'),
    '#default_value' => $settings['commerce_nab_transact_xml_settings']['commerce_nab_xml_password'],
  );
  $form['commerce_nab_transact_xml_settings']['commerce_nab_xml_mode'] = array(
    '#type' => 'select',
    '#title' => t('Transaction mode'),
    '#description' => t('Transaction mode used when sending requests.'),
    '#options' => array(
      'test' => t('Test'),
      'live' => t('Live'),
    ),
    '#default_value' => $settings['commerce_nab_transact_xml_settings']['commerce_nab_xml_mode'],
  );
  $form['commerce_nab_transact_xml_settings']['commerce_nab_xml_txn_type'] = array(
    '#type' => 'select',
    '#title' => t('Transaction type'),
    '#description' => t('Specify the transaction type for XML API transactions.'),
    '#options' => array(
      COMMERCE_NAB_TRANSACT_AUTH_CAPTURE => t('Standard Payment'),
      COMMERCE_NAB_TRANSACT_AUTH_ONLY => t('Preauthorise'),
      // '11' => t('Preauth Complete'),
    ),
    '#default_value' => $settings['commerce_nab_transact_xml_settings']['commerce_nab_xml_txn_type'],
  );

  return $form;
}

/**
 * Payment method callback: checkout form.
 */
function commerce_nab_transact_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');
  return commerce_payment_credit_card_form(array('code' => ''));
}

/**
 * Payment method callback: checkout form validation.
 */
function commerce_nab_transact_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');

  // Validate the credit card fields.
  $settings = array(
    'form_parents' => array_merge($form_parents, array('credit_card')),
  );

  if (!commerce_payment_credit_card_validate($pane_values['credit_card'], $settings)) {
    return FALSE;
  }
}

/**
 * Payment method callback: checkout form submission.
 */
function commerce_nab_transact_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $order_id = $order->order_id;
  
  // We need to truncate the expiry year to two digits eg 2030 -> 30
  $expiry_year = substr($pane_values['credit_card']['exp_year'], -2);
  
  $settings = $payment_method['settings'];
  $data = array(
    'txnType' => $settings['commerce_nab_transact_xml_settings']['commerce_nab_xml_txn_type'],
    'txnSource' => 23,
    'amount' => 100 * commerce_currency_amount_to_decimal($charge['amount'], $charge['currency_code']),
    'currency' => $settings['commerce_nab_currency'],
    'purchaseOrderNo' => $settings['commerce_nab_transact_po_prefix'] . $order_id,
    'CreditCardInfo' => array(
      'cardNumber' => $pane_values['credit_card']['number'],
      'expiryDate' => $pane_values['credit_card']['exp_month'] . '/' . $expiry_year,
    ),
  );

  if (!empty($pane_values['credit_card']['code'])) {
    $data['CreditCardInfo']['cvv'] = $pane_values['credit_card']['code'];
  }

  // Get the next message ID.
  $message_id = variable_get('commerce_nab_transact_xml_message_id', 1);
  variable_set('commerce_nab_transact_xml_message_id', $message_id + 1);
  
  // Build the post XML from the data array.
  $post_data = commerce_nab_transact_xml('Payment', $data, $message_id, $settings);

  // Build the URL where we'll send the request.
  $url = 'https://transact.nab.com.au/' . $settings['commerce_nab_transact_xml_settings']['commerce_nab_xml_mode'] . '/xmlapi/payment';
  
  // Get the response of our payment request.
  if (extension_loaded('openssl')) {
    $response = drupal_http_request($url, array(
                                          'headers' => array('Content-Type' => 'text/xml'),
                                          'method' => 'POST',
                                          'data' => $post_data));
    $response = $response->data;
  }
  // if openssl extension is not loaded we use CURL
  elseif (extension_loaded('curl')) {
    $ch = curl_init($url);

    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $post_data);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);

    $response = curl_exec($ch);
    curl_close($ch);
  }
  
  $xml = simplexml_load_string($response);
  
  // Prepare a transaction object to log the API response.
  $transaction = commerce_payment_transaction_new('nab_transact', $order->order_id);
  $transaction->instance_id = $payment_method['instance_id'];
  $transaction->amount = $charge['amount'];
  $transaction->currency_code = $charge['currency_code'];
  $transaction->payload[REQUEST_TIME] = $response;
  
  watchdog('commerce_nab_transact', 'XML result was: @xml', array('@xml' => $response), WATCHDOG_DEBUG);
  // Check to make sure the response parses and payment passed properly.
  if (isset($xml->Status->statusCode) && $xml->Status->statusCode != '000') {
    $approval = 'No';
    $responsecode = $xml->Status->statusCode;
    $responsetext = $xml->Status->statusDescription;
    
    // Create a failed transaction with the error message.
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
  }
  elseif (isset($xml->Payment->TxnList->Txn->approved)) {
    $approval = $xml->Payment->TxnList->Txn->approved;
    $responsecode = $xml->Payment->TxnList->Txn->responseCode;
    $responsetext = $xml->Payment->TxnList->Txn->responseText;
    $charged = $xml->Payment->TxnList->Txn->amount / 100;
    $txnid = $xml->Payment->TxnList->Txn->txnID;

    $transaction->remote_id = $txnid;
    
    // Set the transaction status based on the type of transaction this was.
    switch ($payment_method['settings']['commerce_nab_transact_xml_settings']['commerce_nab_xml_txn_type']) {
      case COMMERCE_NAB_TRANSACT_AUTH_ONLY:
        $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
        break;

      case COMMERCE_NAB_TRANSACT_AUTH_CAPTURE:
        $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
        break;
    }
  }
  else {
    // Otherwise supply some default values.
    $approval = 'No';
    $responsecode = 'x';

    // If $response is set, we made a request, error was in the response
    if (isset($response)) {
      $responsetext = t('Failed to parse the XML API request or response.');
      // Log the trouble string to the watchdog.
      watchdog('commerce_nab_transact', 'Failed XML parse response:<br/>@xml', array('@xml' => $response), WATCHDOG_ERROR);
    }
    else {
      $responsetext = t('Failed to make the request.');
      // Log the trouble string to the watchdog.
      watchdog('commerce_nab_transact', 'Failed to make the request: "openssl" or "curl" PHP extensions are needed.', array(), WATCHDOG_ERROR);
    }
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
  }
  // Store the type of transaction in the remote status.
  $transaction->remote_status = $responsetext;
  $transaction->message = $responsetext;

  // Save the transaction information.
  commerce_payment_transaction_save($transaction);

  // If the payment failed, display an error and rebuild the form.
  if ($transaction->status == COMMERCE_PAYMENT_STATUS_FAILURE) {
    drupal_set_message(t('We received the following error processing your card. Please enter you information again or try a different card.'), 'error');
    drupal_set_message(check_plain($responsetext), 'error');
    return FALSE;
  }
}

/**
 * Returns the transaction type string for NAB that corresponds to the
 *   Drupal Commerce constant.
 *
 * @param $txn_type
 *   A Drupal Commerce transaction type constant.
 */
function commerce_nab_transact_txn_type($txn_type) {
  switch ($txn_type) {
    case COMMERCE_CREDIT_AUTH_ONLY:
      return 'AUTH_ONLY';
    case COMMERCE_CREDIT_PRIOR_AUTH_CAPTURE:
      return 'PRIOR_AUTH_CAPTURE';
    case COMMERCE_CREDIT_AUTH_CAPTURE:
      return 'AUTH_CAPTURE';
    case COMMERCE_CREDIT_REFERENCE_SET:
    case COMMERCE_CREDIT_REFERENCE_TXN:
    case COMMERCE_CREDIT_REFERENCE_REMOVE:
    case COMMERCE_CREDIT_REFERENCE_CREDIT:
      return NULL;
    case COMMERCE_CREDIT_CREDIT:
      return 'CREDIT';
    case COMMERCE_CREDIT_VOID:
      return 'VOID';
  }
}

/**
 * @return
 *   NAB XML ready for request
 */
function commerce_nab_transact_xml($type, $data, $message_id, $settings) {
  if ($type !== 'Payment' && $type !== 'Echo') {
    return;
  }

  $xml = '<?xml version="1.0" encoding="UTF-8"?><NABTransactMessage>';

  // element: MessageInfo
  $xml .= commerce_nab_transact_message_info($message_id);

  // element: MerchantInfo
  $xml .= commerce_nab_transact_merchant_info($settings);

  // element: RequestType
  $xml .= '<RequestType>' . $type . '</RequestType>';

  // element: Payment
  if ($type == 'Payment') {
    $xml .= commerce_nab_transact_payment_xml($data);
  }

  $xml .= '</NABTransactMessage>';

  return $xml;
}

/**
 * @return
 *   XML MessageInfo element
 */
function commerce_nab_transact_message_info($message_id) {
  return '<MessageInfo><messageID>' . substr(md5($message_id), 0, 30)
        . '</messageID><messageTimestamp>' . commerce_nab_transact_timestamp()
        . '</messageTimestamp><timeoutValue>60</timeoutValue>'
        . '<apiVersion>xml-4.2</apiVersion></MessageInfo>';
}

/**
 * @return
 *   XML MerchantInfo element
 */
function commerce_nab_transact_merchant_info($settings) {
  return '<MerchantInfo><merchantID>' . $settings['commerce_nab_transact_xml_settings']['commerce_nab_xml_merchant_id']
        . '</merchantID><password>' . $settings['commerce_nab_transact_xml_settings']['commerce_nab_xml_password']
        . '</password></MerchantInfo>';
}

/**
 * Build the XML for the payment element.
 *
 * NAB Transact currently only supports 1 payment at a time, hence the hard
 * coded count and ID.
 *
 * @return
 *   XML Payment element
 */
function commerce_nab_transact_payment_xml($data) {
  $xml = '<Payment><TxnList count="1"><Txn ID="1">';

  // Create elements from array
  foreach ($data as $key => $value) {
    if (is_array($value)) {
      $xml .= '<' . $key . '>';
      // Create elements from nested array
      foreach ($value as $arr_key => $arr_value) {
        $xml .= '<' . $arr_key . '>' . $arr_value . '</' . $arr_key . '>';
      }
      $xml .= '</' . $key . '>';
    }
    else {
      $xml .= '<' . $key . '>' . $value . '</' . $key . '>';
    }
  }

  $xml .= '</Txn></TxnList></Payment>';

  return $xml;
}

/**
 * @param $time
 *   (optional) unix timestamp
 *
 * @return
 *   Date in YdmHis000000+000 (php date format)
 */
function commerce_nab_transact_timestamp($time = NULL) {
  if (empty($time)) {
    $time = time();
  }
  // Return a formatted GMT timestamp.
  return date('YdmHis000000+000', $time);
}