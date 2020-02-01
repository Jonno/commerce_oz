<?php

function commerce_oz_postbillpay_logo($aspect = 'port', $size = '_120')
{

	$path = '/' . drupal_get_path('module', 'commerce_oz_postbillpay') . '/images/postbillpay_logo.png';
	return $path;
}



function commerce_oz_postbillpay_admin_settings() {
	
	$form = array();
	
	$descText = '<img src="' . commerce_oz_postbillpay_logo('port', '_120') . '"  align=left />This module provides PostBillPay integration & management for Billers using the Australia Post <a href="http://www.postbillpay.com.au">PostBillPay</a> system.</p>';

  $form['postbillpay_intro'] = array(
    '#type' => 'fieldset',
    '#title' => COMMERCE_OZ_PRODUCT_NAME . t(' PostBillPay'),
    '#collapsible' => FALSE, 
  	'#collapsed' => FALSE,
    '#description' => t($descText),
  );


  $form['postbillpay_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('PostBillPay Credentials'),
    '#collapsible' => TRUE, 
  	'#collapsed' => FALSE,
    '#description' => t('Settings related to your PostBillPay Biller Account.'),
  );
  
  
  $form['postbillpay_settings']['commerce_oz_postbillpay_biller_code'] = array(
    '#type' => 'textfield',
    '#title' => t('PostBillPay Biller Code'),
    '#size' => 24,
    '#description' => t('Your PostBillPay Biller Code as supplied to you by BPAY.'),
    '#default_value' => _commweb_get_var('commerce_oz_postbillpay_biller_code'),
  );
    
  $form['commweb_vpc_about_module'] = array(
   
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
    
  );
  
  return $form;

}

function commerce_oz_postbillpay_admin_settings_validate($form, &$form_state)
{
	return TRUE;
	
}

function commerce_oz_postbillpay_admin_settings_submit($form, &$form_state)
{

	_commweb_set_var('commerce_oz_postbillpay_biller_code', $form_state['values']['commerce_oz_postbillpay_biller_code']);

	
}






