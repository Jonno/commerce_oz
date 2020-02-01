<?php


function commerce_oz_eway_admin_settings() {
	
	$form = array();
	$settings = _commweb_get_var('commerce_oz_gateway_eway');

	$logopath = '/' . drupal_get_path('module', 'commerce_oz') . '/image/eway_68.png';
	
	$descText = '<a href="http://www.eway.com.au"><img src="' . $logopath . '"  align=left /></a>Settings for the <a href="http://www.eway.com.au/">eWay</a> payment service provided by <a href="http://www.eway.com.au">eWay</a> (Australia).<br />It provides both Shared (2 Party) & Direct (3 Party) for processing online credit cards payments.<br /><p>For your protection, this software does not store any credit card details on your servers. All credit card information is handed directly to the Payment Services</p><p>Fill in the appropriate information below and ensure you have SSL enabled for the site.</p>';

  $form['eway_intro_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('eWay'),
    '#collapsible' => FALSE, 
  	'#collapsed' => FALSE,
    '#description' => t($descText),
  );


  $form['eway_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('eWay Merchant Credentials'),
    '#collapsible' => TRUE, 
  	'#collapsed' => FALSE,
    '#description' => t('Settings related to your eWay Merchant Account.'),
  );
  
  $form['eway_settings']['eway_customer_id'] = array(
    '#type' => 'textfield',
    '#title' => t('eWay Customer ID'),
    '#size' => 24,
    '#description' => t('Your eWay Customer ID as supplied to you by eWay. This is required for all eWay payment methods.'),
    '#default_value' => $settings['eway_customer_id'],
  );
  
 
   // Server Hosted (3P) elements  
 
  $form['eway_direct'] = array(
    '#type' => 'fieldset',
    '#title' => t('eWay Direct Payments'),
    '#collapsible' => TRUE, 
  	'#collapsed' => FALSE,
    '#description' => t('Additional settings required for using eWay Direct (2 Party) payment solutions.'),
  );
 
  $form['eway_direct']['eway_customer_login'] = array(
    '#type' => 'textfield',
    '#title' => t('eWay Login Name'),
    '#size' => 45,
    '#maxsize' => 32,
    '#description' => t('Your eWay login. Used for Token payments'),
    '#default_value' => $settings['eway_customer_login'],
  );

  $form['eway_direct']['eway_customer_password'] = array(
    '#type' => 'textfield',
    '#title' => t('eWay Login Password'),
    '#size' => 12,
    '#maxsize' => 15,
    '#description' => t('Your eWay login password. Used for Token payments'),
    '#default_value' => $settings['eway_customer_password'],
  ); 
   
  $form['commerce_oz_submit'] = array(
   
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
    
  );

  
  return $form;

}

function commerce_oz_eway_admin_settings_validate($form, &$form_state)
{
	return TRUE;
	
}

function commerce_oz_eway_admin_settings_submit($form, &$form_state)
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
 	
	_commweb_set_var('commerce_oz_gateway_eway', $settings);
	
}






