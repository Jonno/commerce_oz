<?php

/**
 * @file
 * Include file for gateway and payment method.
 */


/**
 * Implements gateway_provider_info().
 */
function bendigo_gateway_provider_info() {
  $gateway_provider = array();

  $gateway_provider['bendigo'] = array(
    'base' => 'bendigo',
    'title' => t('Bendigo Bank Gateway'),
    'short_title' => t('Bendigo'),
    'description' => t('Process payments with Bendigo Bank'),
    'admin_description' => t('Process payments with Bendigo Bank'),
    'owner' => t('Bendigo Bank of Australia'),
    'uses_migs' => TRUE,
    'active' => TRUE,
    'sales_url' => 'http://www.bendigobank.com.au/',
    'owner_website' => 'http://www.bendigobank.com.au/',
    'service_website' => 'http://www.bendigobank.com.au/',
    'merchant_portal_login' => 'https://migs.mastercard.com.au/ma/login.s?mappedUrl=/bendigo',
    'merchant_portal_name' => 'Bendigo Bank Merchant Administration',
    'payment_methods' => array('bendigo_offsite'),
  );

    return $gateway_provider;
}



/**
 * Implements payment_methods_info().
 */
function bendigo_payment_methods_info() {
  $payment_methods = array();

  $payment_methods['bendigo_offsite'] = array(
    'base' => 'bendigo_3p',
    'title' => t('Bendigo WPG'),
    'gateway_provider' => 'bendigo',
    'short_title' => t('Bendigo WPG'),
    'display_title' => t('Offsite Credit Card using Bendigo Bank servers'),
    'description' => t('Process payments with the Bendigo Bank Servers'),
    'admin_description' => t('Bendigo Bank - Server Hosted (offsite) payments.'),
    'active' => FALSE,
    'terminal' => FALSE, // Allow Admin use from Payments
    'offsite' => TRUE, 
    'service_website' => 'http://www.bendigobank.com.au/',
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
