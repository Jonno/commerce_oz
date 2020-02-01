<?php

/**
 * @file
 * Include file for Payment Express gateway and payment methods.
 */

/**
 * Implements gateway_provider_info().
 */
function paymentexpress_gateway_provider_info() {
  $gateway_provider = array();

  $gateway_provider['paymentexpress'] = array(
    'base' => 'paymentexpress',
    'title' => t('Payment Express Gateway'),
    'short_title' => t('Payment Express'),
    'description' => t('Process payments with Payment Express'),
    'admin_description' => t('Process payments with Payment Express'),
    'owner' => t('DPS Payment Express'),
    'uses_migs' => FALSE,
    'active' => TRUE,
    'sales_url' => 'http://www.paymentexpress.com/Home',
    'owner_website' => 'http://www.paymentexpress.com',
    'service_website' => 'http://www.paymentexpress.com/Home',
    'merchant_portal_login' => 'https://sec.paymentexpress.com/pxmi/logon',
    'merchant_portal_name' => 'Payline Administration',
    'payment_methods' => array('paymentexpress_pxpost', 'paymentexpress_pxpay'),
    'file' => 'includes/api/commerce_oz_paymentexpress_refund',
    'callbacks' => array(
	    	'refund' => 'commerce_oz_paymentexpress_refund',
    	),

  );

    return $gateway_provider;
}

/**
 * Implements payment_methods_info().
 */
function paymentexpress_payment_methods_info() {
  $payment_methods = array();

  $payment_methods['paymentexpress_pxpost'] = array(
    'base' => 'paymentexpress_pxpost',
    'title' => t('Payment Express PxPost'),
    'short_title' => t('Payment Express PxPost'),
    'gateway_provider' => 'paymentexpress',
    'display_title' => t('Payment Express PxPost on this page'),
    'description' => t('Process payments with Payment Express'),
    'admin_description' => t('PxPost - Merchant Hosted (onsite) 2 Party payments.'),
    'active' => FALSE,
    'terminal' => TRUE, // Allow Admin use from Orders
    'offsite' => FALSE, 
    'offsite_autoredirect' => FALSE,
    'file' => 'includes/api/commerce_oz_paymentexpress_2p.inc',
    'callbacks' => array(
	    	'settings_form' => 					'commerce_oz_paymentexpress_2p_settings_form',
	    	'submit_form' => 						'commerce_oz_paymentexpress_2p_submit_form',
	    	'submit_form_submit' => 		'commerce_oz_paymentexpress_2p_submit_form_submit',
	    	'submit_form_validate' => 	'commerce_oz_paymentexpress_2p_submit_form_validate',
    	),
  );

  $payment_methods['paymentexpress_pxpay'] = array(
    'base' => 'paymentexpress_pxpay',
    'title' => t('Payment Express PxPay'),
    'gateway_provider' => 'paymentexpress',
    'short_title' => t('Payment Express PxPay'),
    'display_title' => t('Offsite Credit Card using Payment Express PxPay'),
    'description' => t('Process payments on Payment Express Servers'),
    'admin_description' => t('PxPay - Server Hosted (offsite) 3 Party payments.'),
    'active' => FALSE,
    'terminal' => FALSE, // Don't allow Admin use from Payments
    'offsite' => TRUE, 
    'service_website' => 'http://www.paymentexpress.com/',
    'file' => 'includes/api/commerce_oz_paymentexpress_3p.inc',
    'offsite_autoredirect' => FALSE,
    'callbacks' => array(
	    	'settings_form' => 					'commerce_oz_paymentexpress_3p_settings_form',
	    	'redirect_form' => 					'commerce_oz_paymentexpress_3p_redirect_form',
	    	'redirect_form_submit' => 	'commerce_oz_paymentexpress_3p_redirect_form_submit',
	    	'redirect_form_validate' => 'commerce_oz_paymentexpress_3p_redirect_form_validate',
    	),
    );

  return $payment_methods;
}

