<?php

/**
 * @file
 * Include file for gateway and payment method.
 */


/**
 * Implements payment_methods_info().
 */
function suncorp_payment_methods_info() {
  $payment_methods = array();

  $payment_methods['suncorp_2p'] = array(
    'base' => 'suncorp_2p',
    'title' => t('Suncorp 2 Party'),
    'gateway_provider' => 'suncorp',
    'short_title' => t('Suncorp'),
    'display_title' => t('Credit Card with Suncorp Bank'),
    'description' => t('Process payments with Suncorp Bank '),
    'admin_description' => t('Suncorp Bank - Server Hosted (onsite) 2 Party payments.'),
    'active' => TRUE,
    'terminal' => TRUE, // Allow Admin use from Payments
    'offsite' => TRUE, 
    'offsite_autoredirect' => FALSE,
    'file' => 'commerce_oz_migs_2p.inc',
    'callbacks' => array(
	    	'settings_form' => 					'commerce_oz_migs_2p_settings_form',
	    	'submit_form' => 						'commerce_oz_migs_2p_submit_form',
	    	'submit_form_submit' => 		'commerce_oz_migs_2p_submit_form_submit',
	    	'submit_form_validate' => 	'commerce_oz_migs_2p_submit_form_validate',
    	),
    );


  $payment_methods['suncorp_offsite'] = array(
    'base' => 'suncorp_3p',
    'title' => t('Suncorp 3 Party'),
    'gateway_provider' => 'suncorp',
    'short_title' => t('Suncorp'),
    'display_title' => t('Secure Credit Card using Suncorp Bank'),
    'description' => t('Process payments with Suncorp Bank Servers'),
    'admin_description' => t('Suncorp Bank - Server Hosted (offsite) payments.'),
    'active' => FALSE,
    'terminal' => FALSE, // Allow Admin use from Payments
    'offsite' => TRUE, 
    'offsite_autoredirect' => TRUE,
    );

  return $payment_methods;
}

/**
 * Implements gateway_provider_info().
 */
function suncorp_gateway_provider_info() {
  $gateway_provider = array();

  $gateway_provider['suncorp'] = array(
    'base' => 'suncorp',
    'title' => t('Suncorp Bank Gateway'),
    'short_title' => t('Suncorp Bank'),
    'description' => t('Process payments with Suncorp Bank'),
    'admin_description' => t('Process payments with Suncorp Bank'),
    'owner' => t('Suncorp Bank'),
    'uses_migs' => TRUE,
    'active' => TRUE,
    'sales_url' => 'http://www.suncorpbank.com.au/Business/EFTPOS-and-Online-Payments/Shopping-Cart',
    'owner_website' => 'http://www.suncorpbank.com.au/',
    'service_website' => 'http://www.suncorpbank.com.au/Business/EFTPOS-and-Online-Payments',
    'merchant_portal_login' => 'https://migs.mastercard.com.au/ma/login.s?mappedUrl=/suncorp',
    'merchant_portal_name' => 'Suncorp Virtual POS Administration',
    'payment_methods' => array('suncorp_offsite'),
  );

    return $gateway_provider;
}
