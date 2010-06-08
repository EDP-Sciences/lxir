#! /usr/bin/perl
# Script to generate mathfonts.xml
#

use strict  ;
use XML::LibXML ;

my %MathFontEncodings = (
			 'OML' => 'TeXMathItalicEncoding',
			 'OT1' => 'OT1Encoding',
			 'OMS' => 'TeXMathSymbolEncoding',
			 'OMX' => 'TeXMathExtensionEncoding',
			 'T1'  => 'CorkEncoding',
			 'U'   => 'OT1Encoding',
			);


my %MathFontNames = ('lcmtt' => ['OT1'],
		     'cmtt'  => ['OT1', 'T1'],
		     'cmsy'  => ['OMS'],
		     'cmex'  => ['OMX'],
			 'cmr'   => ['OT1'],
			 'cmm'   => ['OML'],
		     'txr'   => ['OT1', 'T1'],
		     'txmi'  => ['OML'],
			 'txsy'  => ['OMS'],
			 'txsyc' => ['U'],
			 'txex'  => ['OMX'],
		    );

my %FontsSeries = ('m'   => '',
		   'bx'  => 'bold',
		   'b'   => 'bold',
		   'sbx' => 'bold',
		  );
my %FontForms  = ('n'   => 'normal',
		  'it'  => 'italic',
		  'sl'  => 'slanted',
		  'sc'  => 'smallcap',
		  'ui'  => 'normal');

my %OtherFontEncondings = (
			   #'U/example' => 'SpecialEncodingToDefine',
			   #'U/anotherexample' => 'SpecialEncodingToDefine',
			  );

my %PlainTeXFonts = (
		     'normal' => {
				  'tenrm'   => "OT1Encoding",
				  'sevenrm' => "OT1Encoding",
				  'fiverm'  => "OT1Encoding",
				  'tensy'   => "TeXMathSymbolEncoding",
				  'sevensy' => "TeXMathSymbolEncoding",
				  'fivesy'  => "TeXMathSymbolEncoding",
				  'tenex'   => "TeXMathExtensionEncoding",

				},
		     'bolditalic' => {
				  'tenbf'   => "OT1Encoding",
				  'sevenbf' => "OT1Encoding",
				  'fivebf'  => "OT1Encoding",

				    },
		     'italic' => {
				  'teni'    => "TeXMathItalicEncoding",
				  'seveni'  => "TeXMathItalicEncoding",
				  'fivei'   => "TeXMathItalicEncoding",
				},

		   );

my $parser = XML::LibXML->new();
my $commentheader = '%% mathfonts.xml
%% part of lxir - a tool to convert latex documents to xml.
%% Copyright 2007 EDP-Sciences
%
% This work may be distributed and/or modified under the
% conditions of the LaTeX Project Public License, either version 1.3
% of this license or (at your option) any later version.
% The latest version of this license is in
%   http://www.latex-project.org/lppl.txt
% and version 1.3 or later is part of all distributions of LaTeX
% version 2005/12/01 or later.
%
% This work has the LPPL maintenance status `maintained\'.
%
% The Current Maintainer of this work is Xavier Trochu.
%
% This work consists of the files mathfonts.xml.
';

my $doc = XML::LibXML::Document->new('1.0','utf-8');
my $n_commentheader = XML::LibXML::Comment->new($commentheader);
$doc->appendChild($n_commentheader);
my $root = $doc->createElement('encodings');
$doc->setDocumentElement($root);
my $n_comment = XML::LibXML::Comment->new("\n LXir Projet - List of font for math with correspondinf mathml variant\n Generated with gen_mathfonts.pl");
my @Sizes = ('12','11','10','9','8','7','6','5') ;

# Plain TeX Fonts
my $n_comment = XML::LibXML::Comment->new("## Plain TeX Fonts  ###");
$root->appendChild($n_comment);
for my $variant (sort keys  %PlainTeXFonts) {
    for my $fontname (sort keys   %{$PlainTeXFonts{$variant}}) {
	my $elem = $doc->createElement('mathfont');
	$elem->setAttribute('name',$fontname);
	$elem->setAttribute('encoding',$PlainTeXFonts{$variant}->{$fontname});
	$elem->setAttribute('mathvariant',$variant);
	$root->appendChild($elem);
    }
}
for my $name (sort keys %MathFontNames ){
    my $n_comment = XML::LibXML::Comment->new("####### $name #######");
    $root->appendChild($n_comment);
    for my $serie (sort keys %FontsSeries ){
	for my $form ( sort keys %FontForms ) {
	    for my $size (sort {$a <=> $b} @Sizes){
		my $r_enc = $MathFontNames{$name};
		for my $enc (@$r_enc){
			my $fontname = $enc.'/'.$name.'/'.$serie.'/'.$form.'/'.$size;
			#print "$fontname\n" ;
			my $elem = $doc->createElement('mathfont');
			$elem->setAttribute('name',$fontname);
			$elem->setAttribute('encoding',$MathFontEncodings{$enc});
			$elem->setAttribute('mathvariant',$FontsSeries{$serie}.$FontForms{$form});
			$root->appendChild($elem);
		}
	    }
	}
    }
}

# Cas special
for my $font (keys  %OtherFontEncondings){
     my $n_comment = XML::LibXML::Comment->new("####### $font #######");
     $root->appendChild($n_comment);
     for my $serie ( keys %FontsSeries ){
	 for my $form ( keys %FontForms ) {
	     for my $size (@Sizes){
		 my $fontname = $font.'/'.$serie.'/'.$form.'/'.$size;
		 #print "$fontname\n" ;
		 my $elem = $doc->createElement('mathfont');
		 $elem->setAttribute('name',$fontname);
		 $elem->setAttribute('encoding',$OtherFontEncondings{$font});
		 $elem->setAttribute('mathvariant',$FontsSeries{$serie}.$FontForms{$form});
		 $root->appendChild($elem);
	     }
	 }
     }
}

print $doc->toString(1);
