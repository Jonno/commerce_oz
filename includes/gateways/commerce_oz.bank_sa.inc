<?php

/**
 * @file
 * Include file for gateway and payment method.
 */


/**
 * Implements payment_methods_info().
 */
function bank_sa_payment_methods_info() {
  $payment_methods = array();

  $payment_methods['bank_sa_hosted'] = array(
    'base' => 'bank_sa',
    'title' => t('Bank SA'),
    'short_title' => t('Bank SA'),
    'gateway_provider' => 'bank_sa',
    'display_title' => t('Secure Credit Card on this page'),
    'description' => t('Process payments with Bank SA'),
    'admin_description' => t('Bank SA - Server Hosted (offsite) 3 Party payments.'),
    'active' => FALSE,
    'terminal' => FALSE, // Allow Admin use from Payments
    'offsite' => TRUE, 
    'offsite_autoredirect' => FALSE,
  );
  
  return $payment_methods;
}

/**
 * Implements gateway_provider_info().
 */
function bank_sa_gateway_provider_info() {
  $gateway_provider = array();

  $gateway_provider['bank_sa'] = array(
    'base' => 'bank_sa',
    'title' => t('Bank SA Gateway'),
    'short_title' => t('Bank SA'),
    'description' => t('Process payments with Bank SA'),
    'admin_description' => t('Process payments with Bank SA'),
    'owner' => t('Bank SA'),
    'uses_migs' => FALSE,
    'active' => TRUE,
    'sales_url' => 'http://www.banksa.com.au/',
    'owner_website' => 'http://www.banksa.com.au/',
    'service_website' => 'http://www.banksa.com.au/',
    'merchant_portal_login' => 'https://migs.mastercard.com.au/ma/login.s?mappedUrl=/cba',
    'merchant_portal_name' => 'Bank SA Merchant Administration',
    'payment_methods' => array('bank_sa_hosted'),
  );

    return $gateway_provider;
}
