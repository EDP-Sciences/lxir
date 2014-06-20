<?xml version="1.0" encoding="utf-8"?>
<!--
%% pseudomath.xsl
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
% This work consists of the files pseudomath.xsl.
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="xml" indent="no"/>
  <xsl:strip-space elements="math msup msub"/>
<!--  post-process of trivial math -->
<xsl:template match="msup[@single='1']|msub[@single='1']">
  <xsl:copy>
    <mi></mi>
    <xsl:apply-templates/>
  </xsl:copy>
</xsl:template>

  <xsl:template match="math[count(*)=1]">
    <xsl:choose>
      <xsl:when test="msup[@single='1']">
	<xsl:apply-templates mode="simple_math" />
      </xsl:when>
      <xsl:when test="msub[@single='1']">
	<xsl:apply-templates mode="simple_math"/>
      </xsl:when>
      <xsl:otherwise>
	<!-- "true" maths : a make a copy -->
	  <xsl:copy-of select="."/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- templates for "pseudo-math" -->
  <xsl:template match="msup" mode="simple_math">
    <sup><math>
      <xsl:apply-templates/>
    </math></sup>
  </xsl:template>
  <xsl:template match="msub" mode="simple_math">
    <sub><math>
      <xsl:apply-templates/>
    </math></sub>
  </xsl:template>

<!-- Follow recursively the tree: -->
<xsl:template match="*|@*">
  <xsl:copy>
    <xsl:apply-templates select="*|@*|text()"/>
  </xsl:copy>
</xsl:template>

</xsl:stylesheet>
