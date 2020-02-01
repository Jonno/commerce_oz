<?php

/**
 * @file
 * Include file for gateway and payment method.
 */


/**
 * Implements payment_methods_info().
 */
function paymate_payment_methods_info() {
  $payment_methods = array();

  $payment_methods['paymate_2p'] = array(
    'base' => 'paymate_sentinel',
    'title' => t('Paymate Sentinel'),
    'short_title' => t('Paymate'),
    'gateway_provider' => 'paymate',
    'display_title' => t('Paymate Credit Card on this page'),
    'description' => t('Process payments with Paymate'),
    'admin_description' => t('Paymate Sentinel - Merchant Hosted (onsite) 2 Party payments.'),
    'enabled' => FALSE,
    'active' => FALSE,
    'terminal' => FALSE, // Allow Admin use from Payments
    'offsite' => FALSE, 
    'offsite_autoredirect' => FALSE,
    'file' => 'includes/api/commerce_oz.paymate_2p.inc',
    'callbacks' => array(
	    	'settings_form' => 					'commerce_oz_paymate_2p_settings_form',
	    	'submit_form' => 						'commerce_oz_paymate_2p_submit_form',
	    	'submit_form_submit' => 		'commerce_oz_paymate_2p_submit_form_submit',
	    	'submit_form_validate' => 	'commerce_oz_paymate_2p_submit_form_validate',
    	),
  );

  $payment_methods['paymate_express'] = array(
    'base' => 'paymate_express',
    'title' => t('Paymate Express'),
    'gateway_provider' => 'paymate',
    'short_title' => t('Paymate'),
    'display_title' => t('Offsite Credit Card using Paymate Express'),
    'description' => t('Process payments with the Paymate Express Servers'),
    'admin_description' => t('Paymate Express - Server Hosted (offsite) 3 Party payments.'),
    'enabled' => TRUE,
    'active' => FALSE,
    'terminal' => FALSE, // Allow Admin use from Payments
    'offsite' => TRUE, 
    'offsite_autoredirect' => TRUE,    
    'service_website' => 'http://www.paymate.com.au/',
    'file' => 'includes/api/commerce_oz_paymate_3p.inc',
    'callbacks' => array(
	    	'settings_form' => 					'commerce_oz_paymate_3p_settings_form',
	    	'redirect_form' => 					'commerce_oz_paymate_3p_redirect_form',
	    	'redirect_form_submit' => 	'commerce_oz_paymate_3p_redirect_form_submit',
	    	'redirect_form_validate' => 'commerce_oz_paymate_3p_redirect_form_validate',
    	),
    );

  return $payment_methods;
}

/**
 * Implements gateway_provider_info().
 */
function paymate_gateway_provider_info() {
  $gateway_provider = array();

  $gateway_provider['paymate'] = array(
    'base' => 'paymate',
    'title' => t('Paymate Gateway'),
    'short_title' => t('St George'),
    'description' => t('Process payments with Paymate'),
    'admin_description' => t('Process payments with Paymate'),
    'owner' => t('Paymate'),
    'uses_migs' => TRUE,
    'active' => TRUE,
    'sales_url' => 'http://www.paymate.com/',
    'owner_website' => 'http://www.paymate.com.au/',
    'service_website' => 'http://www.paymate.com.au/',
    'merchant_portal_login' => 'https://www.paymate.com/myacct/index.php?option=com_paymatelogin',
    'merchant_portal_name' => 'Paymate Merchant Administration',
    'payment_methods' => array('paymate_express', 'paymate_sentinel'),
  );

    return $gateway_provider;
}
