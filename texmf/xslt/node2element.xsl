<?xml version="1.0" encoding="utf-8"?>
<!--
%% node2element.xsl
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
% This work consists of the files node2element.xsl.
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
		xmlns:lxir="http://www.latex-lxir.org/">
<xsl:output method="xml" indent="yes"/>
<xsl:strip-space elements="node"/>


<xsl:template match="/document">
  <document>
   <xsl:copy-of select="header"/>
    <latex>
      <xsl:apply-templates select="node|text|rule|math"/>
    </latex>
  </document>
</xsl:template>

<xsl:template match="node">
  <xsl:variable name="type">
    <xsl:value-of select="@type"/>
  </xsl:variable>
  <xsl:if test="$type">
    <xsl:element name="{$type}">
      <xsl:apply-templates select="@*|text()|node()"/>
    </xsl:element>
  </xsl:if>
</xsl:template>

<xsl:template match="node/@*">
  <xsl:choose>
    <xsl:when test="name() = 'type'">
    </xsl:when>
    <xsl:otherwise>
      <xsl:attribute name="lxir:{name()}" namespace="http://www.latex-lxir.org">
	<xsl:value-of select="."/>
      </xsl:attribute>
      <!-- <xsl:copy-of select="."/> -->
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>


<xsl:template match="*">
  <xsl:copy-of select="."/>
</xsl:template>

</xsl:stylesheet>