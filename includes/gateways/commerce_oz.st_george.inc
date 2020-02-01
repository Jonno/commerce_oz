<?php

/**
 * @file
 * Include file for gateway and payment method.
 */


/**
 * Implements payment_methods_info().
 */
function st_george_payment_methods_info() {
  $payment_methods = array();

  $payment_methods['st_george_2p'] = array(
    'base' => 'st_george_2p',
    'title' => t('St George 2 Party'),
    'short_title' => t('St George'),
    'gateway_provider' => 'st_george',
    'display_title' => t('Secure Credit Card on this page'),
    'description' => t('Process payments with the St George Bank'),
    'admin_description' => t('St George Bank - Merchant Hosted (onsite) 2 Party payments.'),
    'active' => FALSE,
    'terminal' => FALSE, // Allow Admin use from Payments
    'offsite' => FALSE, 
    'offsite_autoredirect' => FALSE,
  );

  $payment_methods['st_george_3p'] = array(
    'base' => 'st_george_3p',
    'title' => t('St George 3 Party'),
    'gateway_provider' => 'st_george',
    'short_title' => t('St George'),
    'display_title' => t('Secure Credit Card using St George Bank'),
    'description' => t('Process payments with the St George Bank Servers'),
    'admin_description' => t('St George Bank - Server Hosted (offsite) 3 Party payments.'),
    'active' => FALSE,
    'terminal' => FALSE, // Allow Admin use from Payments
    'offsite' => TRUE, 
    'service_website' => 'http://www.cba.com.au/',
    'file' => 'includes/commerce_oz.cba.inc',
    'offsite_autoredirect' => TRUE,
    );

  return $payment_methods;
}

/**
 * Implements gateway_provider_info().
 */
function st_george_gateway_provider_info() {
  $gateway_provider = array();

  $gateway_provider['st_george'] = array(
    'base' => 'st_george',
    'title' => t('St George Bank Gateway'),
    'short_title' => t('St George'),
    'description' => t('Process payments with St George Bank'),
    'admin_description' => t('Process payments with St George Bank'),
    'owner' => t('St George Bank'),
    'uses_migs' => TRUE,
    'active' => TRUE,
    'sales_url' => 'http://www.bendigo.com.au/',
    'owner_website' => 'http://www.bendigo.com.au/',
    'service_website' => 'http://www.bendigo.com.au/',
    'merchant_portal_login' => 'https://migs.mastercard.com.au/ma/login.s?mappedUrl=/cba',
    'merchant_portal_name' => 'St George Merchant Administration',
    'payment_methods' => array('st_george_offsite', 'st_george_onsite'),
  );

    return $gateway_provider;
}
