<!--
%% font2xhtml.xsl
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
% This work consists of the files font2xhtml.xsl.
-->
<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
		xmlns="http://www.w3.org/1999/xhtml" >

  <!-- 

HTML: 
<b>
<i>
<tt>
<sc>
<small>
<large>

CSS: 
font-style: (normal|italic|oblique)
font-weight: (normal|bold|...)
font-size: (xx-small|x-small|small|medium|large|x-large|xx-large)
font-variant: (normal|small-caps)
font-family: (serif|sans-serif|monospace)

LaTeX:
series: bf, md
family: rm, sf, tt
shape: up, sl, sc, it
size: tiny,scriptsize,footnotesize,small,normalsize,large,Large,LARGE,huge,Huge

  -->

  <!-- 
       Dans cette version, on utilise du html "de base", sans css : c'est un peu plus simple
       et ça permet de mettre au point les algo pour la transformation vers une DTD de type
       JournalPublishing .
  -->

  <xsl:template match="text">
    <!-- size si elle est présente, puis recursivement family,serie, shape -->
    <xsl:choose>
      <xsl:when test="@size">
	<xsl:apply-templates select="@size"/>
      </xsl:when>
      <xsl:when test="@family">
	<xsl:apply-templates select="@family"/>
      </xsl:when>

      <xsl:otherwise>
	<span class="text"><xsl:value-of select="."/></span>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="text/@size">
    <xsl:choose>
      <xsl:when test="string(.)='tiny' or string(.)='scriptsize' or string(.)='footnotesize' or string(.)='small'">
	<small>
	  <xsl:apply-templates select="../@family"/>
	</small>
      </xsl:when>
      <xsl:when test="string(.)='large' or string(.)='Large' or string(.)='LARGE' or string(.)='huge' or string(.)='Huge' ">
	<large>
	  <xsl:apply-templates select="../@family"/>
	</large>
      </xsl:when>
       <xsl:otherwise>
	 <xsl:apply-templates select="../@family"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="text/@family">
    <xsl:choose>
      <xsl:when test="string(.)='tt'">
	<tt>
	  <xsl:apply-templates select="../@series"/>
	</tt>
      </xsl:when>
      <xsl:otherwise>
	<xsl:apply-templates select="../@series"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="text/@series">
    <xsl:choose>
      <xsl:when test="string(.)='bx'">
	<b>
	  <xsl:apply-templates select="../@shape"/>
	</b>
      </xsl:when>
      <xsl:otherwise>
	<xsl:apply-templates  select="../@shape"/>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template match="text/@shape">
    <xsl:choose>
      <xsl:when test="string(.)='it'">
	<i>
	  <span class="text"><xsl:value-of select=".."/></span>
	</i>
      </xsl:when>
       <xsl:when test="string(.)='sl'">
	 <i>
	   <span class="text"><xsl:value-of select=".."/></span>
	 </i>
      </xsl:when>
      <xsl:when test="string(.)='sc'">
	<sc>
	  <span class="text"><xsl:value-of select=".."/></span>
	</sc>
      </xsl:when>
     <xsl:otherwise>
       <span class="text"><xsl:value-of select=".."/></span>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
</xsl:stylesheet>
