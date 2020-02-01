<?php


function commerce_oz_directone_admin_settings($form, &$form_state) {
	
	$form = array();
	
	/* Server Hosted Credentials Settings */
	
	$settings = _commweb_get_var('commerce_oz_gateway_directone');

	$logopath = '/' . drupal_get_path('module', 'commerce_oz') . '/image/directone_68.png';
	
	$descText = '<a href="http://www.directone.com.au"><img src="' . $logopath . '" /></a><br />Settings for the <a href="http://www.directone.com.au/">Direct One</a> payment gateway.<br />Direct One provides <a href="http://www.directone.com.au/html/welcome/services.html#hosted_payments">Direct One Hosted</a> payments solutions for processing online credit card payments.<br />';

  $form['intro_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Direct One'),
    '#collapsible' => FALSE, 
  	'#collapsed' => FALSE,
    '#description' => t($descText),
  );


   /* Merchant Hosted Credentials Settings */

  $form['directone_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Direct One Credentials'),
    '#collapsible' => TRUE, 
  	'#collapsed' => TRUE,
    '#description' => t('Settings for your Direct One account. This are not the login details for your Merchant portal.'),
  );
  
  $form['directone_settings']['directone_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Direct One Merchant Name'),
    '#size' => 24,
    '#description' => t('Your Direct One Merchant Name as supplied to you by Direct One.'),
    '#default_value' => $settings['directone_username'],
  );
  
/*  $form['directone_settings']['directone_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Direct One Access Code'),
    '#size' => 12,
    '#description' => t('Your PX Post Password for making transactions as supplied to you by Direct One.'),
    '#default_value' => $settings['directone_password'],
  );
 */
 
 // module_load_include('inc',  'commerce_oz',  'includes/commerce_oz.global');
  
 // $form += commerce_oz_global_admin_settings_form($settings);
  
  
  $form['commerce_oz_submit'] = array(
   
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
    
  );
  
  return $form;

}

function commerce_oz_directone_admin_settings_validate($form, &$form_state)
{
	
	return true;
}

function commerce_oz_directone_admin_settings_submit($form, &$form_state)
{
 	
 	$settings = $form_state['values'];
 	unset($settings['form_build_id']);
 	unset($settings['form_token']);
 	unset($settings['form_id']);
 	unset($settings['op']);
 	unset($settings['commerce_oz_submit']);
 	
	_commweb_set_var('commerce_oz_gateway_directone', $settings);
	
}






