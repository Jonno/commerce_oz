<?php

function commerce_oz_bpay_logo($aspect = 'port', $size = '_120')
{

	$path = '/' . drupal_get_path('module', 'commerce_oz_bpay') . '/images/bpay_' . $aspect  . $size . '.png';
	return $path;
}


function commerce_oz_bpay_admin_settings() {
	
	$form = array();
	
	$descText = '<img src="' . commerce_oz_bpay_logo('port', '_120') . '"  align=left />This module provides BPAY integration & management for Billers using the BPAY system.</p>';

  $form['bpay_intro'] = array(
    '#type' => 'fieldset',
    '#title' => COMMERCE_OZ_PRODUCT_NAME . t(' BPAY'),
    '#collapsible' => FALSE, 
  	'#collapsed' => FALSE,
    '#description' => t($descText),
  );


  $form['bpay_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('BPAY Credentials'),
    '#collapsible' => TRUE, 
  	'#collapsed' => FALSE,
    '#description' => t('Settings related to your BPAY Biller Account.'),
  );
  
  
  $form['bpay_settings']['commerce_oz_bpay_biller_code'] = array(
    '#type' => 'textfield',
    '#title' => t('BPAY Biller Code'),
    '#size' => 24,
    '#description' => t('Your BPAY Biller Code as supplied to you by BPAY.'),
    '#default_value' => _commweb_get_var('commerce_oz_bpay_biller_code'),
  );
    
  $form['commweb_vpc_about_module'] = array(
   
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
    
  );
  
  return $form;

}

function commerce_oz_bpay_admin_settings_validate($form, &$form_state)
{
	return TRUE;
	
}

function commerce_oz_bpay_admin_settings_submit($form, &$form_state)
{

	_commweb_set_var('commerce_oz_bpay_biller_code', $form_state['values']['commerce_oz_bpay_biller_code']);

	
}






