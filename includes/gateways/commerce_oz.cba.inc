<?php

/**
 * @file
 * Include file for gateway and payment method.
 */

/**
 * Implements gateway_provider_info().
 */
function cba_gateway_provider_info() {
 // $gateway_provider = array();

  $gateway_provider['cba'] = array(
    'base' => 'cba',
    'title' => t('CommWeb Gateway'),
    'short_title' => t('CommWeb'),
    'description' => t('Process payments with CommWeb'),
    'admin_description' => t('Process payments with CommWeb'),
    'owner' => t('Commonwealth Bank of Australia'),
    'uses_migs' => TRUE,
    'active' => TRUE,
    'sales_url' => 'http://www.cba.com.au/',
    'settings_url' => 'admin/commerce/config/commerce_commweb',
    'owner_website' => 'http://www.cba.com.au/',
    'service_website' => 'http://www.cba.com.au/',
    'merchant_portal_login' => 'https://migs.mastercard.com.au/ma/login.s?mappedUrl=/cba',
    'merchant_portal_name' => 'CommWeb Merchant',
    'payment_methods' => array('commerce_commweb', 'commerce_commweb_3p'),
    'file' => 'includes/api/commerce_oz_migs_refund',
    'callbacks' => array(
	    	'refund' => 'commerce_oz_migs_refund',
    	),
  );

    return $gateway_provider;
}

/**
 * Implements payment_methods_info().
 */
function cba_payment_methods_info() {
 // $payment_methods = array();

  $payment_methods['commerce_commweb'] = array(
    'base' => 'commerce_commweb',
    'title' => t('CommWeb VPC'),
    'short_title' => t('CommWeb VPC'),
    'gateway_provider' => 'cba',
    'display_title' => t('Credit Card via CommWeb'),
    'description' => t('Process payments with the CommWeb VPC'),
    'admin_description' => t('CommWeb VPC - Merchant Hosted (onsite) 2 Party payments.'),
    'active' => FALSE,
    'terminal' => TRUE, // Allow Admin use from Payments
    'offsite' => FALSE, 
    'offsite_autoredirect' => FALSE,
    'file' => 'includes/api/commerce_oz_migs_2p.inc',
    'callbacks' => array(
	    	'settings_form' => 					'commerce_oz_migs_2p_settings_form',
	    	'submit_form' => 						'commerce_oz_migs_2p_submit_form',
	    	'submit_form_submit' => 		'commerce_oz_migs_2p_submit_form_submit',
	    	'submit_form_validate' => 	'commerce_oz_migs_2p_submit_form_validate',
    	),
  );

  $payment_methods['commerce_commweb_3p'] = array(
    'base' => 'commerce_commweb_3p',
    'title' => t('CommWeb 3 Party'),
    'gateway_provider' => 'cba',
    'short_title' => t('CommWeb 3P'),
    'display_title' => t('Secure Credit Card using CommBank Servers'),
    'description' => t('Process payments with the CommWeb Payment Servers'),
    'admin_description' => t('CommWeb 3P - Server Hosted (offsite) 3 Party payments.'),
    'active' => FALSE,
    'terminal' => FALSE, // Allow Admin use from Payments
    'offsite' => TRUE, 
    'offsite_autoredirect' => TRUE,
    'file' => 'includes/api/commerce_oz_migs_3p.inc',
    'callbacks' => array(
	    	'settings_form' => 					'commerce_oz_migs_3p_settings_form',
	    	'redirect_form' => 					'commerce_oz_migs_3p_redirect_form',
	    	'redirect_form_submit' => 	'commerce_oz_migs_3p_redirect_form_submit',
	    	'redirect_form_validate' => 'commerce_oz_migs_3p_redirect_form_validate',
    	),
    );

  return $payment_methods;
}


