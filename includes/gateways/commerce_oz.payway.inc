<?php

/**
 * @file
 * Include file for gateway and payment method.
 */


/**
 * Implements payment_methods_info().
 */
function payway_payment_methods_info() {
  $payment_methods = array();

  $payment_methods['payway_api'] = array(
    'base' => 'payway_api',
    'title' => t('PayWay API'),
    'short_title' => t('Westpac PayWay'),
    'gateway_provider' => 'payway',
    'display_title' => t('Westpac PayWay on this page'),
    'description' => t('Process payments with Westpac PayWay API'),
    'admin_description' => t('PayWay API - Merchant Hosted (onsite) 2 Party payments.'),
    'active' => FALSE,
    'terminal' => TRUE, // Allow Admin use from Payments
    'offsite' => FALSE, 
    'offsite_autoredirect' => FALSE,
    'file' => 'includes/api/commerce_oz_payway_api.inc',
    'callbacks' => array(
	    	'settings_form' => 					'commerce_oz_payway_api_settings_form',
	    	'submit_form' => 						'commerce_oz_payway_api_submit_form',
	    	'submit_form_submit' => 		'commerce_oz_payway_api_submit_form_submit',
	    	'submit_form_validate' => 	'commerce_oz_payway_api_submit_form_validate',
    	),
    	
  );

  $payment_methods['payway_net'] = array(
    'base' => 'payway_net',
    'title' => t('PayWay Net'),
    'gateway_provider' => 'payway',
    'short_title' => t('PayWay Net'),
    'display_title' => t('Offsite using Westpac PayWay Servers'),
    'description' => t('Process payments with Westpac PayWay Net'),
    'admin_description' => t('PayWay Net - Server Hosted (offsite) 3 Party payments.'),
    'active' => FALSE,
    'terminal' => TRUE, // Allow Admin use from Payments
    'offsite' => TRUE, 
    'service_website' => 'http://www.payway.com.au/',
    'file' => 'includes/commerce_oz.payway.inc',
    'offsite_autoredirect' => TRUE,
    );

  return $payment_methods;
}

/**
 * Implements gateway_provider_info().
 */
function payway_gateway_provider_info() {
  $gateway_provider = array();

  $gateway_provider['payway'] = array(
    'base' => 'payway',
    'title' => t('Westpac Payway'),
    'short_title' => t('Payway'),
    'description' => t('Process payments with Westpac Payway'),
    'admin_description' => t('Process payments with Westpac Payway'),
    'owner' => t('Westpac Bank'),
    'uses_migs' => FALSE,
    'active' => TRUE,
    'sales_url' => 'http://www.payway.com.au/',
    'settings_url' => 'admin/commerce/config/commerce_payway',
    'owner_website' => 'http://www.payway.com.au/',
    'service_website' => 'http://www.payway.com.au/',
    'merchant_portal_login' => 'https://www.payway.com.au/core/LoginView',
    'merchant_portal_name' => 'Payway Merchant Administration',
    'payment_methods' => array('payway_api', 'payway_net'),    
    'file' => 'includes/api/commerce_oz_payway_refund',
    'callbacks' => array(
	    	'refund' => 'commerce_oz_payway_refund',
    	),
  );

    return $gateway_provider;
}
