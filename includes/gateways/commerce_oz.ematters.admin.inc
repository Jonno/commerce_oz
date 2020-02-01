<?php


function commerce_oz_ematters_admin_settings() {
	
	$form = array();
	$settings = _commweb_get_var('commerce_oz_gateway_ematters');

	$logopath = '/' . drupal_get_path('module', 'commerce_oz') . '/image/ematters_68.png';
	
	$descText = '<a href="http://www.ematters.com.au"><img src="' . $logopath . '"  align=left /></a>Settings for the <a href="http://www.ematters.com.au/">eMatters</a> payment service provided by <a href="http://www.ematters.com.au">eMatters</a> (Australia).<br />It provides both Shared (2 Party) & Direct (3 Party) for processing online credit cards payments.<br /><p>For your protection, this software does not store any credit card details on your servers. All credit card information is handed directly to the Payment Services</p><p>Fill in the appropriate information below and ensure you have SSL enabled for the site.</p>';

  $form['ematters_info_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('eMatters'),
    '#collapsible' => FALSE, 
  	'#collapsed' => FALSE,
    '#description' => t($descText),
  );


  $form['ematters_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('eMatters Merchant Credentials'),
    '#collapsible' => TRUE, 
  	'#collapsed' => FALSE,
    '#description' => t('Settings related to your eMatters Merchant Account.'),
  );
  
  $form['ematters_settings']['ematters_customer_id'] = array(
    '#type' => 'textfield',
    '#title' => t('eMatters Customer ID'),
    '#size' => 24,
    '#description' => t('Your eMatters Customer ID as supplied to you by eWay. This is required for all eMatters payment methods.'),
    '#default_value' => $settings['ematters_customer_id'],
  );
  
  $form['ematters_settings']['ematters_poster_password'] = array(
    '#type' => 'textfield',
    '#title' => t('eMatters Posting Password'),
    '#size' => 24,
    '#description' => t('Your Posting password. This is setup by you in your eMatters portal.'),
    '#default_value' => $settings['ematters_poster_password'],
  );
  
  $form['ematters_settings']['ematters_refund_pin'] = array(
    '#type' => 'textfield',
    '#title' => t('eMatters Refund PIN'),
    '#size' => 10,
    '#description' => t('Your Refund PIN. This is setup by you in your eMatters portal.'),
    '#default_value' => $settings['ematters_refund_pin'],
  );
  
  
  
  module_load_include('inc',  'commerce_oz',  'includes/commerce_oz.global');
  
  $form += commerce_oz_global_admin_settings_form($settings);
   
  $form['commerce_oz_submit'] = array(
   
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
    
  );

  
  return $form;

}

function commerce_oz_ematters_admin_settings_validate($form, &$form_state)
{
	return TRUE;
	
}

function commerce_oz_ematters_admin_settings_submit($form, &$form_state)
{
	 //	// dsm($form_state, 'form state');
	// 	// dsm($form, 'form');

 	
 	$settings = $form_state['values'];
 	unset($settings['form_build_id']);
 	unset($settings['form_token']);
 	unset($settings['form_id']);
 	unset($settings['op']);
 	unset($settings['commerce_oz_submit']);
 	
 //	// dsm($settings, '$settings');
 	
	_commweb_set_var('commerce_oz_gateway_ematters', $settings);
	
}






