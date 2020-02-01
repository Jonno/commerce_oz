<?php

/**
 * @file
 * Include file for eMatters gateway and payment method.
 */

/**
 * Implements gateway_provider_info().
 */
function ematters_gateway_provider_info() {
 // $gateway_provider = array();

  $gateway_provider['ematters'] = array(
    'base' => 'ematters',
    'title' => t('eMatters Gateway'),
    'short_title' => t('eMatters'),
    'description' => t('Process payments with eMatters'),
    'admin_description' => t('Process payments with eMatters'),
    'owner' => t('eMatters'),
    'uses_migs' => FALSE,
    'active' => TRUE,
    'sales_url' => 'http://www.ematters.com.au/',
    'owner_website' => 'http://www.ematters.com.au/',
    'service_website' => 'http://www.ematters.com.au/',
    'merchant_portal_login' => 'https://merchant.ematters.com.au/ematters.nsf?login',
    'merchant_portal_name' => 'eMatters Secure Merchant',
    'payment_methods' => array('ematters_api', 'ematters_shared'),
    'file' => 'includes/api/commerce_oz_ematters_refund',
    'callbacks' => array(
	    	'refund' => 'commerce_oz_ematters_refund',
    	),
  );

    return $gateway_provider;
}

/**
 * Implements payment_methods_info().
 */
function ematters_payment_methods_info() {
 
 $payment_methods = array();


  $payment_methods['ematters_api'] = array(
    'base' => 'ematters_api',
    'title' => t('eMatters Merchant'),
    'short_title' => t('eMatters Merchant'),
    'gateway_provider' => 'ematters',
    'display_title' => t('Credit Card via eMatters Merchant on this page'),
    'description' => t('Process payments with eMatters Merchant'),
    'admin_description' => t('eMatters Merchant - Merchant Hosted (onsite) payments.'),
    'active' => FALSE,
    'terminal' => TRUE, // Allow Admin use from Payments
    'offsite' => FALSE, 
    'offsite_autoredirect' => FALSE,
    'file' => 'includes/api/commerce_oz_ematters_2p.inc',
    'callbacks' => array(
	    	'settings_form' => 					'commerce_oz_ematters_2p_settings_form',
	    	'submit_form' => 						'commerce_oz_ematters_2p_submit_form',
	    	'submit_form_submit' => 		'commerce_oz_ematters_2p_submit_form_submit',
	    	'submit_form_validate' => 	'commerce_oz_ematters_2p_submit_form_validate',
    	),
  );

  $payment_methods['ematters_shared'] = array(
    'base' => 'ematters_shared',
    'title' => t('eMatters Shared'),
    'gateway_provider' => 'ematters',
    'short_title' => t('eMatters Shared'),
    'display_title' => t('Credit Card with eMatters Servers'),
    'description' => t('Process payments with the eMatters Servers'),
    'admin_description' => t('eMatters Shared - Server Hosted (offsite) payments.'),
    'active' => FALSE,
    'terminal' => FALSE, // Allow Admin use from Payments
    'offsite' => TRUE, 
    'offsite_autoredirect' => FALSE,
    'service_website' => 'http://www.ematters.com.au/',
    'file' => 'includes/api/commerce_oz_ematters_3p.inc',
    'callbacks' => array(
	    	'settings_form' => 					'commerce_oz_ematters_3p_settings_form',
	    	'redirect_form' => 					'commerce_oz_ematters_3p_redirect_form',
	    	'redirect_form_submit' => 	'commerce_oz_ematters_3p_redirect_form_submit',
	    	'redirect_form_validate' => 'commerce_oz_ematters_3p_redirect_form_validate',
    	),
    );
	//dsm($payment_methods, 'ematters pm');
	
  return $payment_methods;
}


