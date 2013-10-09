<?xml version="1.0" encoding="utf-8"?>
<!--
%% float2xhtml.xsl
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
% This work consists of the files float2xhtml.xsl.
-->
<xsl:stylesheet version="1.0"
		xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
		xmlns="http://www.w3.org/1999/xhtml"
		xmlns:lxir="http://www.latex-lxir.org">

<xsl:template match="float">
    <div class="{name()}">
	<xsl:apply-templates select="@*"/>
	<xsl:choose>
	    <xsl:when test="par/centering">
		<center>
		    <xsl:apply-templates/>
		</center>
	    </xsl:when>
	    <xsl:otherwise>
		<xsl:apply-templates/>
	    </xsl:otherwise>
	</xsl:choose>
    </div>
    <!-- line break for better debug -->
    <xsl:text>&#xa;</xsl:text>
</xsl:template>

<xsl:template match="float/par">
    <xsl:apply-templates select="*"/>
</xsl:template>

<xsl:template match="float/par/centering"/>

<xsl:template match="caption">
  <div class="{name()}">
      <a>
	<xsl:attribute name="name">
	  <xsl:text>capt_id_</xsl:text>
	  <xsl:value-of select="@lxir:id"/>
	</xsl:attribute>
      </a>
	  <xsl:apply-templates select="captionText|captionMark|par/captionText|par/captionMark|hyperlinkurl/captionMark|par/hyperlinkurl/captionMark" />
  </div>
    <!-- line break for better debug -->
    <xsl:text>&#xa;</xsl:text>
</xsl:template>

<xsl:template match="captionMark|captionText">
  <span class="{name()}">
    <xsl:if test="@lxir:language">
      <xsl:attribute name="xml:lang"><xsl:value-of select="@lxir:language" /></xsl:attribute>
    </xsl:if>
    <xsl:apply-templates/>
  </span>
</xsl:template>


<!-- environnement dont le taggage est redondant avec float -->
<xsl:template match="figure">
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="table">
  <xsl:apply-templates/>
</xsl:template>

</xsl:stylesheet>
