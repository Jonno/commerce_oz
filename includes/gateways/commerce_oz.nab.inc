<?php

/**
 * @file
 * Include file for gateway and payment method.
 */


/**
 * Implements payment_methods_info().
 */
function nab_payment_methods_info() {
  $payment_methods = array();

  $payment_methods['nab_transact'] = array(
    'base' => 'nab_transact',
    'title' => t('NAB Transact'),
    'short_title' => t('NAB Transact'),
    'gateway_provider' => 'nab',
    'display_title' => t('Secure Credit Card on this page'),
    'description' => t('Process payments with NAB Transact'),
    'admin_description' => t('NAB Transact - Merchant Hosted (onsite) 2 Party payments.'),
    'active' => FALSE,
    'terminal' => FALSE, // Allow Admin use from Payments
    'offsite' => FALSE, 
    'offsite_autoredirect' => FALSE,
    'file' => 'includes/commerce_oz_transact_2p.inc',
    'callbacks' => array(
	    	'settings_form' => 					'commerce_oz_transact_settings_form',
	    	'submit_form' => 						'commerce_oz_migs_2p_submit_form',
	    	'submit_form_submit' => 		'commerce_oz_migs_2p_submit_form_submit',
	    	'submit_form_validate' => 	'commerce_commweb_3p_submit_form_validate',
    	),
  );
  
  return $payment_methods;
}

/**
 * Implements gateway_provider_info().
 */
function nab_gateway_provider_info() {
  $gateway_provider = array();

  $gateway_provider['nab'] = array(
    'base' => 'nab',
    'title' => t('NAB Transact Gateway'),
    'short_title' => t('NAB Transact'),
    'description' => t('Process payments with NAB Transact'),
    'admin_description' => t('Process payments with NAB Transact'),
    'owner' => t('National Australia Bank'),
    'uses_migs' => FALSE,
    'active' => TRUE,
    'sales_url' => 'http://www.nab.com.au/',
    'owner_website' => 'http://www.nab.com.au/',
    'service_website' => 'http://www.nab.com.au/',
    'merchant_portal_login' => 'https://migs.mastercard.com.au/ma/login.s?mappedUrl=/cba',
    'merchant_portal_name' => 'NAB Merchant Administration',
    'payment_methods' => array('nab_transact'),
  );

    return $gateway_provider;
}
