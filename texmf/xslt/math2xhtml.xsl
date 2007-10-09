<?xml version="1.0" encoding="utf-8"?>
<!--
%% math2xhtml.xsl
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
% This work consists of the files math2xhtml.xsl.
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
		xmlns="http://www.w3.org/1999/xhtml"
		xmlns:mm="http://www.w3.org/1998/Math/MathML"
		>
  <xsl:template match="mm:math">
    <span class="math">
      <xsl:copy-of select="."/>
    </span>
  </xsl:template>

  <xsl:template match="equation">
    <div class="{name()}">
      <xsl:attribute name="eqNum">
	<xsl:value-of select="eqNum/@num"/>
      </xsl:attribute>
      <xsl:if test="label">
	<xsl:attribute name="eqLabel">
	  <xsl:value-of select="label/@idlabel"/>
	</xsl:attribute>
	<xsl:apply-templates select="label"/>
      </xsl:if>
      <xsl:apply-templates select="mm:math"/>
      <span class="eqNum">
	<xsl:value-of select="eqNum/@num"/>
      </span>
    </div>
  </xsl:template>

  <!-- pour une raison bizarre, si on ne lui précise pas le ns, 
  il met parfois (??) un ns vide -->
  <xsl:template match="sup|sub">
    <xsl:element name="{name()}" namespace="http://www.w3.org/1999/xhtml">
      <xsl:apply-templates/>
    </xsl:element>
  </xsl:template>
</xsl:stylesheet>