<!--
%% fonttext.xsl
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
% This work consists of the files fonttext.xsl.
-->
<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" 
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:lxir="http://www.latex-lxir.org">
  <xsl:output method="xml" indent="no"/>

<!-- Introduce font names and properties in text element attributes-->
<xsl:template match="text">
  <xsl:variable name="fontnum">
    <xsl:value-of select="@font"/>
  </xsl:variable>
  <xsl:copy>
    <xsl:copy-of select="@*"/>
    <xsl:for-each select="/document/header/font[@index=$fontnum]/fontattributes/@*[name()='lxir:family' or name()='lxir:size' or name()='lxir:series' or name()='lxir:shape']">
      <xsl:copy-of select="."/>
    </xsl:for-each>
    <xsl:apply-templates/>
  </xsl:copy>

</xsl:template>


<xsl:template match="fontListMatching"/>
<xsl:template match="/document/header"/>
<!-- Follow recursively the tree: -->
<xsl:template match="*|@*">
  <xsl:copy>
    <xsl:apply-templates select="*|@*|text()"/>
  </xsl:copy>
</xsl:template>


</xsl:stylesheet>