<!--
%% params.xsl
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
% This work consists of the files params.xsl.
-->
<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="xml" indent="yes"/>

<!-- Stylesheet for processing params of the LXir XML which already have
submit a node2element.xsl and clean.xsl transformation-->


<!-- Follow recursively the tree: -->
<xsl:template match="*|@*">
  <!-- params element merging -->
  <!-- TODO -->
  <xsl:choose>
    <xsl:when test="params">
      <xsl:copy>
	<xsl:apply-templates select="params/@*"/>
	<xsl:apply-templates select="text()|*[name()!='params']|@*"/>
      </xsl:copy>
    </xsl:when>
    <xsl:otherwise>
      <xsl:copy>
	<xsl:apply-templates select="text()|*|@*"/>
      </xsl:copy>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template match="param/@*">
  <xsl:copy-of select="."/>
</xsl:template>



</xsl:stylesheet>
