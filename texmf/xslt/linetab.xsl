<!--
%% linetab.xsl
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
% This work consists of the files linetab.xsl.
-->
<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="xml" indent="no"/>
<!-- Stylesheet for transforming rule inside tabular or array in 
     vline (vertical lines) or hline (horizontal lines) 
     or sline (quare line, which should very seldom arise....)
     This is based on a and a attributes: 
     * if a < b then it's a hline
     * if a = b then it's a vline
     * if a = b then it's a sline .... 
-->
<xsl:template match="tabular/rule|multicolumn/rule">
  <xsl:variable name="elementName">
    <xsl:choose>
      <xsl:when test="@a &lt; @b">
	<xsl:text>hline</xsl:text>
      </xsl:when>
      <xsl:when test="@a &gt; @b">
	<xsl:text>vline</xsl:text>
      </xsl:when>
      <xsl:otherwise>
	<xsl:text>sline</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:variable>
  <xsl:element name="{$elementName}">
    <xsl:apply-templates/>
  </xsl:element>
</xsl:template>

<!-- Follow recursively the tree: -->
<xsl:template match="*|@*">
  <xsl:copy>
    <xsl:apply-templates select="*|@*|text()"/>
  </xsl:copy>
</xsl:template>


</xsl:stylesheet>
