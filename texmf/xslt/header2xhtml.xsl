<?xml version="1.0" encoding="utf-8"?>
<!--
%% header2xhtml.xsl
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
% This work consists of the files header2xhtml.xsl.
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
		xmlns:lxir="http://www.latex-lxir.org"
		xmlns="http://www.w3.org/1999/xhtml" >
  <xsl:output method="xml" indent="no"/>
  <xsl:template name="header">
    <a name="top"/>
    <div class="header">
      <xsl:apply-templates select="//*[@lxir:header='yes']"/>
      <xsl:apply-templates select="//LaTeXClass"/>	
      <xsl:apply-templates select="//maketitle"/>	
         </div>
  </xsl:template>

<xsl:template match="title|subtitle|author|date|abstract|resume">
  <div class="{name(.)}">
    <xsl:attribute name="lxir:header">
      <xsl:value-of select="@lxir:header"/>
    </xsl:attribute>
    <xsl:apply-templates/>
  </div>
</xsl:template>

<xsl:template match="author/and">
  <xsl:text> -- </xsl:text>
</xsl:template>

</xsl:stylesheet>
