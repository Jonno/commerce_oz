<?php

/**
 * @file
 * Include file for gateway and payment method.
 */


/**
 * Implements payment_methods_info().
 */
function securepay_payment_methods_info() {
  $payment_methods = array();

  $payment_methods['securepay_api'] = array(
    'base' => 'securepay_api',
    'title' => t('SecurePay API'),
    'short_title' => t('SecurePay'),
    'gateway_provider' => 'securepay',
    'display_title' => t('Secure Credit Card on this page'),
    'description' => t('Process payments with SecurePay'),
    'admin_description' => t('SecurePay - Merchant Hosted (onsite) 2 Party payments.'),
    'active' => FALSE,
    'terminal' => FALSE, // Allow Admin use from Payments
    'offsite' => FALSE, 
    'offsite_autoredirect' => FALSE,
  );

  $payment_methods['securepay_hosted'] = array(
    'base' => 'securepay_hosted',
    'title' => t('SecureBill Hosted'),
    'gateway_provider' => 'securepay',
    'short_title' => t('SecurePay'),
    'display_title' => t('Secure Credit Card using SecurePay'),
    'description' => t('Process payments with the SecurePay Servers'),
    'admin_description' => t('SecurePay - Server Hosted (offsite) 3 Party payments.'),
    'active' => FALSE,
    'terminal' => FALSE, // Allow Admin use from Payments
    'offsite' => TRUE, 
    'service_website' => 'http://www.securepay.com.au/',
    'file' => 'includes/commerce_oz.cba.inc',
    'offsite_autoredirect' => TRUE,
    );

  return $payment_methods;
}

/**
 * Implements gateway_provider_info().
 */
function securepay_gateway_provider_info() {
  $gateway_provider = array();

  $gateway_provider['securepay'] = array(
    'base' => 'securepay',
    'title' => t('SecurePay Gateway'),
    'short_title' => t('SecurePay'),
    'description' => t('Process payments with SecurePay'),
    'admin_description' => t('Process payments with SecurePay'),
    'owner' => t('Paymate'),
    'uses_migs' => TRUE,
    'active' => TRUE,
    'sales_url' => 'http://www.securepay.com/',
    'owner_website' => 'http://www.securepay.com.au/',
    'service_website' => 'http://www.securepay.com.au/',
    'merchant_portal_login' => 'http://www.securepay.com.au/products-services/reporting/merchant-login/',
    'merchant_portal_name' => 'SecurePay Merchant',
    'payment_methods' => array('securepay_api', 'securepay_hosted'),
  );

    return $gateway_provider;
}
