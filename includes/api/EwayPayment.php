<?php
 /*
  * EwayPayment.php
  * Electronic Payment XML Interface for eWAY
  *
  * (c) Copyright Matthew Horoschun, CanPrint Communications 2005.
  *
  * $Id: EwayPayment.php,v 1.2 2005/04/18 03:30:33 matthew Exp $
  *
  * Date:    2005-04-18
  * Version: 2.0
  *
  */

define( 'EWAY_DEFAULT_GATEWAY_URL', 'https://www.eway.com.au/gateway_cvn/xmlpayment.asp' );
define( 'EWAY_DEFAULT_CUSTOMER_ID', '87654321' );

define( 'EWAY_CURL_ERROR_OFFSET', 1000 );
define( 'EWAY_XML_ERROR_OFFSET',  2000 );

define( 'EWAY_TRANSACTION_OK',       0 );
define( 'EWAY_TRANSACTION_FAILED',   1 );
define( 'EWAY_TRANSACTION_UNKNOWN',  2 );


class EwayPayment {
    var $parser;
    var $xmlData;
    var $currentTag;
    
    var $ewayGatewayURL;
    var $ewayCustomerID;
    
    var $ewayTotalAmount;
    var $ewayCustomerFirstname;
    var $ewayCustomerLastname;
    var $ewayCustomerEmail;
    var $ewayCustomerAddress;
    var $ewayCustomerPostcode;
    var $ewayCustomerInvoiceDescription;
    var $ewayCustomerInvoiceRef;
    var $ewayCardHoldersName;
    var $ewayCardNumber;
    var $ewayCardExpiryMonth;
    var $ewayCardExpiryYear;
    var $ewayTrxnNumber;
    var $ewayOption1;
    var $ewayOption2;
    var $ewayOption3;
    var $CVN;

    var $ewayResultTrxnStatus;
    var $ewayResultTrxnNumber;
    var $ewayResultTrxnOption1;
    var $ewayResultTrxnOption2;
    var $ewayResultTrxnOption3;
    var $ewayResultTrxnReference;
    var $ewayResultTrxnError;
    var $ewayResultAuthCode;
    var $ewayResultReturnAmount;
    
    var $ewayError;
    var $ewayErrorMessage;

    /***********************************************************************
     *** XML Parser - Callback functions                                 ***
     ***********************************************************************/
    function epXmlElementStart ($parser, $tag, $attributes) {
        $this->currentTag = $tag;
    }
    
    function epXmlElementEnd ($parser, $tag) {
        $this->currentTag = "";
    }
    
    function epXmlData ($parser, $cdata) {
        $this->xmlData[$this->currentTag] = $cdata;
    }
    
    /***********************************************************************
     *** SET values to send to eWAY                                      ***
     ***********************************************************************/
    function setCustomerID( $customerID ) {
        $this->ewayCustomerID = $customerID;
    }
    
    function setTotalAmount( $totalAmount ) {
        $this->ewayTotalAmount = $totalAmount;
    }
    
    function setCustomerFirstname( $customerFirstname ) {
        $this->ewayCustomerFirstname = $customerFirstname;
    }
    
    function setCustomerLastname( $customerLastname ) {
        $this->ewayCustomerLastname = $customerLastname;
    }
    
    function setCustomerEmail( $customerEmail ) {
        $this->ewayCustomerEmail = $customerEmail;
    }
    
    function setCustomerAddress( $customerAddress ) {
        $this->ewayCustomerAddress = $customerAddress;
    }
    
    function setCustomerPostcode( $customerPostcode ) {
        $this->ewayCustomerPostcode = $customerPostcode;
    }
    
    function setCustomerInvoiceDescription( $customerInvoiceDescription ) {
        $this->ewayCustomerInvoiceDescription = $customerInvoiceDescription;
    }
    
    function setCustomerInvoiceRef( $customerInvoiceRef ) {
        $this->ewayCustomerInvoiceRef = $customerInvoiceRef;
    }
    
    function setCardHoldersName( $cardHoldersName ) {
        $this->ewayCardHoldersName = $cardHoldersName;
    }
    
    function setCardNumber( $cardNumber ) {
        $this->ewayCardNumber = $cardNumber;
    }
    
    function setCardExpiryMonth( $cardExpiryMonth ) {
        $this->ewayCardExpiryMonth = $cardExpiryMonth;
    }
    
    function setCardExpiryYear( $cardExpiryYear ) {
        $this->ewayCardExpiryYear = $cardExpiryYear;
    }
    
    function setTrxnNumber( $trxnNumber ) {
        $this->ewayTrxnNumber = $trxnNumber;
    }
    
    function setOption1( $option1 ) {
        $this->ewayOption1 = $option1;
    }
    
    function setOption2( $option2 ) {
        $this->ewayOption2 = $option2;
    }
    
    function setOption3( $option3 ) {
        $this->ewayOption3 = $option3;
    }

    function setCVN( $CVN ) {
        $this->ewayCVN = $CVN;
    }

    /***********************************************************************
     *** GET values returned by eWAY                                     ***
     ***********************************************************************/
    function getTrxnStatus() {
        return $this->ewayResultTrxnStatus;
    }
    
    function getTrxnNumber() {
        return $this->ewayResultTrxnNumber;
    }
    
    function getTrxnOption1() {
        return $this->ewayResultTrxnOption1;
    }
    
    function getTrxnOption2() {
        return $this->ewayResultTrxnOption2;
    }
    
    function getTrxnOption3() {
        return $this->ewayResultTrxnOption3;
    }
    
    function getTrxnReference() {
        return $this->ewayResultTrxnReference;
    }
    
    function getTrxnError() {
        return $this->ewayResultTrxnError;
    }
    
    function getAuthCode() {
        return $this->ewayResultAuthCode;
    }
    
    function getReturnAmount() { 
        return $this->ewayResultReturnAmount;
    }

    function getCVN() { 
        return $this->ewayCVN;
    }

    function getError()
    {
        if( $this->ewayError != 0 ) {
            // Internal Error
            return $this->ewayError;
        } else {
            // eWAY Error
            if( $this->getTrxnStatus() == 'True' ) {
                return EWAY_TRANSACTION_OK;
            } elseif( $this->getTrxnStatus() == 'False' ) {
                return EWAY_TRANSACTION_FAILED;
            } else {
                return EWAY_TRANSACTION_UNKNOWN;
            }
        }
    }

    function getErrorMessage()
    {
        if( $this->ewayError != 0 ) {
            // Internal Error
            return $this->ewayErrorMessage;
        } else {
            // eWAY Error
            return $this->getTrxnError();
        }
    }

    /***********************************************************************
     *** Class Constructor                                               ***
     ***********************************************************************/
    function EwayPayment( $customerID = EWAY_DEFAULT_CUSTOMER_ID, $gatewayURL = EWAY_DEFAULT_GATEWAY_URL ) {
        $this->ewayCustomerID = $customerID;
        $this->ewayGatewayURL = $gatewayURL;
    }

    /***********************************************************************
     *** Business Logic                                                  ***
     ***********************************************************************/
    function doPayment() {
        $xmlRequest = "<ewaygateway>".
                "<ewayCustomerID>".htmlentities( $this->ewayCustomerID )."</ewayCustomerID>".
                "<ewayTotalAmount>".htmlentities( $this->ewayTotalAmount)."</ewayTotalAmount>".
                "<ewayCustomerFirstName>".htmlentities( $this->ewayCustomerFirstname )."</ewayCustomerFirstName>".
                "<ewayCustomerLastName>".htmlentities( $this->ewayCustomerLastname )."</ewayCustomerLastName>".
                "<ewayCustomerEmail>".htmlentities( $this->ewayCustomerEmail )."</ewayCustomerEmail>".
                "<ewayCustomerAddress>".htmlentities( $this->ewayCustomerAddress )."</ewayCustomerAddress>".
                "<ewayCustomerPostcode>".htmlentities( $this->ewayCustomerPostcode )."</ewayCustomerPostcode>".
                "<ewayCustomerInvoiceDescription>".htmlentities( $this->ewayCustomerInvoiceDescription )."</ewayCustomerInvoiceDescription>".
                "<ewayCustomerInvoiceRef>".htmlentities( $this->ewayCustomerInvoiceRef )."</ewayCustomerInvoiceRef>".
                "<ewayCardHoldersName>".htmlentities( $this->ewayCardHoldersName )."</ewayCardHoldersName>".
                "<ewayCardNumber>".htmlentities( $this->ewayCardNumber )."</ewayCardNumber>".
                "<ewayCardExpiryMonth>".htmlentities( $this->ewayCardExpiryMonth )."</ewayCardExpiryMonth>".
                "<ewayCardExpiryYear>".htmlentities( $this->ewayCardExpiryYear )."</ewayCardExpiryYear>".
                "<ewayTrxnNumber>".htmlentities( $this->ewayTrxnNumber )."</ewayTrxnNumber>".
                "<ewayCVN>".htmlentities( $this->ewayCVN )."</ewayCVN>".
                "<ewayOption1>".htmlentities( $this->ewayOption1 )."</ewayOption1>".
                "<ewayOption2>".htmlentities( $this->ewayOption2 )."</ewayOption2>".
                "<ewayOption3>".htmlentities( $this->ewayOption3 )."</ewayOption3>".
                "</ewaygateway>";

        /* Use CURL to execute XML POST and write output into a string */
        $ch = curl_init( $this->ewayGatewayURL );
        curl_setopt( $ch, CURLOPT_POST, 1 );
        curl_setopt( $ch, CURLOPT_POSTFIELDS, $xmlRequest );
        curl_setopt( $ch, CURLOPT_RETURNTRANSFER, 1 );
        curl_setopt( $ch, CURLOPT_TIMEOUT, 240 );
        curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
        $xmlResponse = curl_exec( $ch );
        
        // Check whether the curl_exec worked.
        if( curl_errno( $ch ) == CURLE_OK ) {
            // It worked, so setup an XML parser for the result.
            $this->parser = xml_parser_create();
            
            // Disable XML tag capitalisation (Case Folding)
            xml_parser_set_option ($this->parser, XML_OPTION_CASE_FOLDING, FALSE);
            
            // Define Callback functions for XML Parsing
            xml_set_object($this->parser, $this);
            xml_set_element_handler ($this->parser, "epXmlElementStart", "epXmlElementEnd");
            xml_set_character_data_handler ($this->parser, "epXmlData");
            
            // Parse the XML response
            xml_parse($this->parser, $xmlResponse, TRUE);
            
            if( xml_get_error_code( $this->parser ) == XML_ERROR_NONE ) {
                // Get the result into local variables.
                $this->ewayResultTrxnStatus = isset($this->xmlData['ewayTrxnStatus']) ? $this->xmlData['ewayTrxnStatus']:"";
                $this->ewayResultTrxnNumber = isset($this->xmlData['ewayTrxnNumber']) ? $this->xmlData['ewayTrxnNumber']:"";
                $this->ewayResultTrxnOption1 = isset($this->xmlData['ewayTrxnOption1']) ? $this->xmlData['ewayTrxnOption1']:"";
                $this->ewayResultTrxnOption2 = isset($this->xmlData['ewayTrxnOption2']) ? $this->xmlData['ewayTrxnOption2']:"";
                $this->ewayResultTrxnOption3 = isset($this->xmlData['ewayTrxnOption3']) ? $this->xmlData['ewayTrxnOption3']:"";
                $this->ewayResultTrxnReference = isset($this->xmlData['ewayTrxnReference']) ? $this->xmlData['ewayTrxnReference']:"";
                $this->ewayResultAuthCode = isset($this->xmlData['ewayAuthCode']) ? $this->xmlData['ewayAuthCode']:"";
                $this->ewayResultReturnAmount = isset($this->xmlData['ewayReturnAmount']) ? $this->xmlData['ewayReturnAmount']:"";
                $this->ewayResultTrxnError = isset($this->xmlData['ewayTrxnError']) ? $this->xmlData['ewayTrxnError']:"";
                $this->ewayError = 0;
                $this->ewayErrorMessage = '';
            } else {
                // An XML error occured. Return the error message and number.
                $this->ewayError = xml_get_error_code( $this->parser ) + EWAY_XML_ERROR_OFFSET;
                $this->ewayErrorMessage = xml_error_string( $ewayError );
            }
            // Clean up our XML parser
            xml_parser_free( $this->parser );
        } else {
            // A CURL Error occured. Return the error message and number. (offset so we can pick the error apart)
            $this->ewayError = curl_errno( $ch ) + EWAY_CURL_ERROR_OFFSET;
            $this->ewayErrorMessage = curl_error( $ch );
        }
        // Clean up CURL, and return any error.
        curl_close( $ch );
        return $this->getError();
    }
}
?>
