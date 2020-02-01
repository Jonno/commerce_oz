<?php

/**
 * @file
 * Include file for Payment Express gateway and payment methods.
 */

/**
 * Implements gateway_provider_info().
 */
function directone_gateway_provider_info() {
  $gateway_provider = array();

  $gateway_provider['directone'] = array(
    'base' => 'directone',
    'title' => t('DirectOne Payment Solutions'),
    'short_title' => t('DirectOne'),
    'description' => t('Process payments with DirectOne'),
    'admin_description' => t('Process payments with DirectOne'),
    'owner' => t('DirectOne'),
    'uses_migs' => FALSE,
    'active' => TRUE,
    'refunds' => FALSE,
    'sales_url' => 'http://www.directone.com/',
    'owner_website' => 'http://www.directone.com',
    'service_website' => 'http://www.directone.com/Home',
    'merchant_portal_login' => 'https://vault.directone.com.au/members/account_access.html',
    'merchant_portal_name' => 'DirectOne Members',
    'payment_methods' => array('directone_hosted'),

  );

    return $gateway_provider;
}

/**
 * Implements payment_methods_info().
 */
function directone_payment_methods_info() {
  $payment_methods = array();

  $payment_methods['directone_hosted'] = array(
    'base' => 'directone_hosted',
    'title' => t('DirectOne Payments Solutions'),
    'short_title' => t('DirectOne'),
    'gateway_provider' => 'directone',
    'display_title' => t('DirectOne on hosted servers'),
    'description' => t('Process payments with DirectOne'),
    'admin_description' => t('DirectOne - Hosted (onsite) 3 Party payments.'),
    'active' => FALSE,
    'terminal' => FALSE, // Allow Admin use from Orders
    'offsite' => TRUE, 
    'offsite_autoredirect' => TRUE,
    'file' => 'includes/api/commerce_oz_directone.inc',
    'callbacks' => array(
	    	'settings_form' => 					'commerce_oz_directone_settings_form',
	    	'redirect_form' => 					'commerce_oz_directone_redirect_form',
	    	'redirect_form_submit' => 	'commerce_oz_directone_redirect_form_submit',
	    	'redirect_form_validate' => 'commerce_oz_directone_redirect_form_validate',    
	    	),
  );

  return $payment_methods;
}

