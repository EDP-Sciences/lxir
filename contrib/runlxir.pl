#! /usr/bin/perl -w
# lxir - a tool to convert latex documents to xml.
# Copyright (C) 2007  EDP-Sciences
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

use strict ;
use Getopt::Long ;

my $VERSION  = '1.0' ;
my ($SCRIPT_NAME) = ($0 =~ m!^.*?([^/]+)$!); 
# For latex, we add -max-print-line=1000 in the command line:
# this is for MikTeX 2.5 and up, where it seems that it is not possible
# to configure this parameter. 
# For tetex/texlive, this option is invalid, but there no harm 
# to add it anyway!
my $LaTeXCommand = 'latex  -halt-on-error -max-print-line=1000 ' ;
my $LXirCommand  = 'lxir '; 

my $LaTeXLogSuff = '_texlxir.log';
my $File     = undef ; # input LaTeX filename
my $FileLXir = undef; # LaTeX file  with the \RequirePackage{lxir}
my $DVILXir  = undef; # tagged DVI filename
my $XMLLXir  = undef ; # output XML filename 
my $LXirLogSuff = '_runlxir.log' ;

sub runlatex{
    info(" * latex compilation");
    _complileLaTeX($File);
     infoOK();
    
}
sub runlatexwithTags{
    info(" *  latex compilation with LXir tags");
    _complileLaTeX($FileLXir);
    $DVILXir = $FileLXir ;
    $DVILXir =~ s/\.tex$/.dvi/ ;
    if (-r $DVILXir && -s $DVILXir) {
	infoOK();
    } else {
	errorHandling("DVI file is empty.",1 ) ;
    }
    
}

sub _complileLaTeX{
    my $file = shift ;
    my $filelog = $file ;
    $filelog =~ s/\.tex/$LaTeXLogSuff/ ;
    my $command = "$LaTeXCommand $file &> $filelog" ;
    for my $i (0..2) {
	system($command) == 0 or do {
	    errorHandling("File ``$file'': LaTeX compilation failed: see $filelog",1);
	};
   }   
}

sub makeLXirReady{
    $FileLXir = $File ;
    $FileLXir =~ s/\.tex$/_lxir.tex/;
    errorHandling("Bug !",2) if ($FileLXir eq $File) ;
    info(" *  Adding \\RequirePackage{lxir}");
     open HF, "<$File" or 
      errorHandling("Unable to read ``$File'': $!") ;
    open HFX, ">$FileLXir" or 
      errorHandling("Unable to open ``$File'' for writting: $!") ;
    my $documentclassFound = undef ;
    while (<HF>) {
	my $tmp = $_ ;
	# clean comments
	$tmp =~ s/(^|[^\\])\%.*$//;
	if ( (!$documentclassFound) 
	     && $tmp =~ m/\\documentclass\s*[\{|\[]/) 
	  {
	      $documentclassFound = 1 ;
	      print HFX '\RequirePackage{lxir}',"\n" ;
	  }
	print HFX ;

    }
    close HF ;
    close HFX ;
    if ($documentclassFound ) {
       	infoOK();
    } else {
	errorHandling("Unable to find any \\documentclass in ``$File''",1) ;
    }
}

sub runlxir{
    unless ($XMLLXir) {
	$XMLLXir = $File ;
	$XMLLXir =~ s/\.tex$/.xml/ ;
    }
    
    info(" * DVI -> XML");
    my $filelog = $XMLLXir ;
    if ($filelog =~ m/\.xml$/i) {
	$filelog =~ s/\.xml$/$LXirLogSuff/i
    } else {
	$filelog .= $LXirLogSuff ;
    }
    my $command = "lxir $DVILXir  > $XMLLXir 2>  $filelog" ;
    system($command) == 0 or 
      errorHandling("lxir $FileLXir failed: see $filelog",1) ;
    infoOK() ;
}

sub errorHandling{
    my $msg  = shift ;
    my $exit = shift ;
    print STDERR "[$SCRIPT_NAME Error] $msg\n" ;
    exit $exit if $exit ;
}

sub info{
   my $msg  = shift ;
   print STDERR "[$SCRIPT_NAME info] $msg\n" ;
}

sub infoOK{
    info("    -> OK");
}
# MAIN 

print STDERR "$SCRIPT_NAME (version $VERSION)\n";
print STDERR "More information on http://www.latex-lxir.org\n" ;
my $result = GetOptions('--output=s' => \$XMLLXir);
$File = shift @ARGV ;

unless ($File){
    print STDERR "Usage:\n $SCRIPT_NAME \[-o file.xml] file.tex\n" ;
    exit ;
}
errorHandling("Unable to read the file ``$File''",1) 
  unless  (-r $File) ;

errorHandling("Your LaTeX filename must have a .tex extension",1)
  unless $File =~ m/\.tex$/;
runlatex($File) ;
makeLXirReady();
runlatexwithTags($FileLXir);
runlxir() ;
info("LXir XML file: $XMLLXir") ;


=pod 

=head1 NAME

runlxir.pl

=head1 SYNOPSIS

runlxir.pl [-o <altfile.xml>] <file.tex>

=head1 DESCRIPTION

runlxir.pl perform the necessary steps to produce an XML file out of a LaTeX document. 
 
By default, the XML data will be output to <file>.xml. You can choose wathever filename 
you want by using -o option.

Please read the LXir documentation on http://www.latex-lxir.org.

=head1 AUTHOR

(c)  EDP Sciences - Jean-Paul Jorda 

=head1 SEE ALSO

The LXir Web Site http://www.latex-lxir.org


=cut

