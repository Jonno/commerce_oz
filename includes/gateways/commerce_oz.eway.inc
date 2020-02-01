<?php

/**
 * @file
 * Include file for gateway and payment method.
 */


/**
 * Implements payment_methods_info().
 */
function eway_payment_methods_info() {
 // $payment_methods = array();

  $payment_methods['eway_rapid'] = array(
    'base' => 'eway_rapid',
    'title' => t('eWay Rapid 3.0'),
    'short_title' => t('eWay Rapid 3.0'),
    'gateway_provider' => 'eWay',
    'display_title' => t('Secure Credit Card on this page'),
    'description' => t('Process payments with eWay Rapid 3.0'),
    'admin_description' => t('eWay Rapid 3.0 - Merchant Hosted (onsite) 2 Party payments.'),
    'active' => FALSE,
    'terminal' => TRUE, // Allow Admin use from Payments
    'offsite' => FALSE, 
    'offsite_autoredirect' => FALSE,
    'file' => 'includes/api/commerce_oz_eway_rapid.inc',
    'callbacks' => array(
	    	'settings_form' => 					'commerce_oz_eway_rapid_settings_form',
	    	'submit_form' => 						'commerce_oz_eway_rapid_submit_form',
	    	'submit_form_submit' => 		'commerce_oz_eway_rapid_submit_form_submit',
	    	'submit_form_validate' => 	'commerce_oz_eway_rapid_submit_form_validate',
    	),
  );

  $payment_methods['eway_direct'] = array(
    'base' => 'eway_direct',
    'title' => t('eWay Direct'),
    'short_title' => t('eWay Direct'),
    'gateway_provider' => 'eWay',
    'display_title' => t('Credit Card via eWay on this page'),
    'description' => t('Process payments with eWay Direct'),
    'admin_description' => t('eWay Direct - Merchant Hosted (onsite) 2 Party payments.'),
    'active' => FALSE,
    'terminal' => TRUE, // Allow Admin use from Payments
    'offsite' => FALSE, 
    'offsite_autoredirect' => FALSE,
    'file' => 'includes/api/commerce_oz_eway_2p.inc',
    'callbacks' => array(
	    	'settings_form' => 					'commerce_oz_eway_2p_settings_form',
	    	'submit_form' => 						'commerce_oz_eway_2p_submit_form',
	    	'submit_form_submit' => 		'commerce_oz_eway_2p_submit_form_submit',
	    	'submit_form_validate' => 	'commerce_oz_eway_2p_submit_form_validate',
    	),
  );

  $payment_methods['eway_shared'] = array(
    'base' => 'eway_shared',
    'title' => t('eWay Shared'),
    'gateway_provider' => 'eway',
    'short_title' => t('eWay Shared'),
    'display_title' => t('Credit Card with eWay Servers'),
    'description' => t('Process payments with the eWay Servers Payment Servers'),
    'admin_description' => t('eWay Shared - Server Hosted (offsite) 3 Party payments.'),
    'active' => FALSE,
    'terminal' => TRUE, // Allow Admin use from Payments
    'offsite' => TRUE, 
    'offsite_autoredirect' => TRUE,
    'service_website' => 'http://www.eway.com.au/',
    'file' => 'includes/api/commerce_oz_eway_3p.inc',
    'callbacks' => array(
	    	'settings_form' => 					'commerce_oz_eway_3p_settings_form',
	    	'redirect_form' => 					'commerce_oz_eway_3p_redirect_form',
	    	'redirect_form_submit' => 	'commerce_oz_eway_3p_redirect_form_submit',
	    	'redirect_form_validate' => 'commerce_oz_eway_3p_redirect_form_validate',
    	),
    );

  return $payment_methods;
}

/**
 * Implements gateway_provider_info().
 */
function eway_gateway_provider_info() {
 // $gateway_provider = array();

  $gateway_provider['eway'] = array(
    'base' => 'eway',
    'title' => t('eWay Gateway'),
    'short_title' => t('eWay'),
    'description' => t('Process payments with eWay'),
    'admin_description' => t('Process payments with eWay'),
    'owner' => t('eWay'),
    'uses_migs' => FALSE,
    'active' => TRUE,
    'sales_url' => 'http://www.eway.com.au/',
    'settings_url' => 'admin/commerce/config/commerce_commweb',
    'owner_website' => 'http://www.eway.com.au/',
    'service_website' => 'http://www.eway.com.au/',
    'merchant_portal_login' => 'https://au.myeway.com/Login.aspx',
    'merchant_portal_name' => 'eWay Merchant Administration',
    'payment_methods' => array('eway_shared', 'eway_direct'),
  );

    return $gateway_provider;
}
