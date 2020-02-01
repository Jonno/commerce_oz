<?php

/**
 * @file
 * Include file for eMatters gateway and payment method.
 */

/**
 * Implements gateway_provider_info().
 */
function poli_gateway_provider_info() {
 // $gateway_provider = array();

  $gateway_provider['poli'] = array(
    'base' => 'poli',
    'title' => t('POLi Gateway'),
    'short_title' => t('POLi Payments'),
    'description' => t('Bank transfer with POLi'),
    'admin_description' => t('Process payments with POLi'),
    'owner' => t('POLi Payments'),
    'uses_migs' => FALSE,
    'active' => TRUE,
    'sales_url' => 'http://www.polipayments.com',
    'owner_website' => 'http://www.polipayments.com',
    'service_website' => 'http://www.polipayments.com/',
    'merchant_portal_login' => 'https://consoles.apac.paywithpoli.com/Login.aspx',
    'merchant_portal_name' => 'POLi Console Login',
    'payment_methods' => array('poli_transfer'),
  );

    return $gateway_provider;
}

/**
 * Implements payment_methods_info().
 */
function poli_payment_methods_info() {
 // $payment_methods = array();


  $payment_methods['poli_transfer'] = array(
    'base' => 'poli_transfer',
    'title' => t('POLi Bank Transfers'),
    'gateway_provider' => 'poli',
    'short_title' => t('POLi Payments'),
    'display_title' => t('Bank transfer with POLi Payments'),
    'description' => t('Process payments with POLi bank transfers'),
    'admin_description' => t('POLi - Bank transfer (offsite) payments.'),
    'active' => FALSE,
    'terminal' => FALSE, // Allow Admin use from Payments
    'offsite' => TRUE, 
    'offsite_autoredirect' => FALSE,
    
    'file' => 'includes/api/commerce_oz_poli.inc',
    'callbacks' => array(
	    	'settings_form' => 					'commerce_oz_poli_settings_form',
	    	'submit_form' => 						'commerce_oz_poli_submit_form',
	    	'submit_form_submit' => 		'commerce_oz_poli_submit_form_submit',
	    	'submit_form_validate' => 	'commerce_oz_poli_submit_form_validate',
	    	'redirect_form' => 					'commerce_oz_poli_redirect_form',
	    	'redirect_form_submit' => 	'commerce_oz_poli_redirect_form_submit',
	    	'redirect_form_validate' => 'commerce_oz_poli_redirect_form_validate',
    	),
    );

  return $payment_methods;
}


