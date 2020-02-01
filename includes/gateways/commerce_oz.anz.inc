<?php

/**
 * @file
 * Include file for gateway and payment method.
 */


/**
 * Implements hook_commerce_payment_methods_info().
 */
function anz_payment_methods_info() {
  $payment_methods = array();

  $payment_methods['anz_egate'] = array(
    'base' => 'anz_egate',
    'title' => t('ANZ eGate 2 Party'),
    'short_title' => t('ANZ eGate'),
    'gateway_provider' => 'anz',
    'display_title' => t('Credit Card via ANZ eGate'),
    'description' => t('Process payments with ANZ eGate'),
    'admin_description' => t('ANZ eGate - Merchant Hosted (onsite) 2 Party payments.'),
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

  $payment_methods['anz_egate_3p'] = array(
    'base' => 'anz_egate_3p',
    'title' => t('ANZ eGate 3 Party'),
    'gateway_provider' => 'anz',
    'short_title' => t('ANZ eGate 3 Party'),
    'display_title' => t('Secure Credit Card using ANZ Servers'),
    'description' => t('Process payments with the eGate Payment Servers'),
    'admin_description' => t('ANZ eGate - Server Hosted (offsite) 3 Party payments.'),
    'active' => FALSE,
    'terminal' => FALSE, // Allow Admin use from Payments
    'offsite' => TRUE, 
    'service_website' => 'http://www.anz.com.au/',
    'offsite_autoredirect' => false,
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

/**
 * Implements gateway_provider_info().
 */
function anz_gateway_provider_info() {
  $gateway_provider = array();

  $gateway_provider['anz'] = array(
    'base' => 'anz',
    'title' => t('ANZ eGate Gateway'),
    'short_title' => t('ANZ eGate'),
    'description' => t('Process payments with ANZ eGate'),
    'admin_description' => t('Process payments with ANZ eGate'),
    'owner' => t('Australia New Zealand Bank'),
    'uses_migs' => TRUE,
    'active' => TRUE,
    'sales_url' => 'http://www.anz.com.au/small%2Dbusiness/products%2Dservices/merchant%2Dservices/internet%2Dpayment%2Dsolutions/',
    'owner_website' => 'http://www.anz.com.au/',
    'merchant_portal_login' => 'https://migs.mastercard.com.au/ma/ANZAU',
    'merchant_portal_name' => 'ANZ eGate Merchant Administration',
    'service_website' => 'http://www.anz.com.au/',
    'payment_methods' => array('anz_egate', 'anz_egate_3p'),
    'file' => 'includes/api/commerce_oz_migs_refund',
    'callbacks' => array(
	    	'refund' => 'commerce_oz_migs_refund',
    	),
  );

    return $gateway_provider;
}


/**
 * Implements gateway_provider_info().
 */
function anz_payment_method_settings_form() {
 $form = array();
	
	$logopath = '/' . drupal_get_path('module', 'commerce_commweb') . '/image/' . 'anz' . '_68.png';
	
	$descText = '<a href="http://www.anz.com.au"><img src="' . $logopath . '" /></a><br />Integrates the <a href="http://www.anz.com.au/">ANZ eGate for Merchant Hosted Payment</a> (2 party). There are no user defined settings here.';

  $form['intro_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('ANZ eGate'),
    '#collapsible' => FALSE, 
  	'#collapsed' => FALSE,
    '#description' => t($descText),
  );

  return $form;
}




