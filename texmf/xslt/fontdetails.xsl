<?xml version="1.0" encoding="utf-8"?>
<!--
%% fontdetails.xsl
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
% This work has the LPPL maintenance status `maintained'.
% 
% The Current Maintainer of this work is Jean-Paul Jorda.
%
% This work consists of the files fontdetails.xsl.
-->
<xsl:stylesheet version="1.0" 
		xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
		xmlns:lxir="http://www.latex-lxir.org">
  <xsl:output method="xml" indent="no"/>

  <xsl:template match="header/font">
    <xsl:variable name="fontnum">
      <xsl:value-of select="@index"/>
    </xsl:variable>
    <xsl:variable name="fontname">
      <xsl:value-of select="name"/>
    </xsl:variable>
    <xsl:copy>
      <xsl:apply-templates select="@*"/>
      <!-- on recherche dans fontList si une fonte correspond (@name=$fontname)
	   Dans le cas de  fontes "scaler", c'est n'est pas le cas.
	   On se base alors sur la 1ere partie du nom
	   (name="t1xbss at 23.63593pt" => t1xbss)
	   La taille n'est alors pas récupérable (de cette façon au moins), puisque t1xbss 
	   correspond potentiellement à des tailles différentes.
      -->
      <xsl:choose>
	<xsl:when test="//fontList/fontDescription[@lxir:name=$fontname]">
	  <fontattributes>
	    <xsl:for-each select="//fontList/fontDescription[@lxir:name=$fontname]/@*[name()='lxir:family' or name()='lxir:size' or name()='lxir:series' or name()='lxir:shape']">
	      <xsl:copy-of select="."/>
	    </xsl:for-each>
	  </fontattributes>
	</xsl:when>
	<xsl:when test="//fontList/fontDescription[substring-before(@lxir:name,' ')=$fontname]">
	  <fontattributes>
	  <xsl:for-each select="//fontList/fontDescription[substring-before(@lxir:name,' ')=$fontname]/@*[name()='lxir:family' or name()='lxir:series' or name()='lxir:shape']">
	    <xsl:copy-of select="."/>
	  </xsl:for-each>
	  </fontattributes>
	  
	</xsl:when>
      </xsl:choose>
      <xsl:apply-templates select="*"/>
    </xsl:copy>
  </xsl:template>

<xsl:template match="fontList"/>


<!-- Follow recursively the tree: -->
<xsl:template match="*|@*">
  <xsl:copy>
    <xsl:apply-templates select="*|@*|text()"/>
  </xsl:copy>
</xsl:template>


</xsl:stylesheet>