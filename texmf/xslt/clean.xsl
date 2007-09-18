<!--
%% clean.xsl
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
% This work consists of the files clean.xsl.
-->
<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="xml" indent="yes"/>

<!-- 
     Stylesheet for cleaning the LXir XML which already have
     submit a 
     node2element.xsl+params.xsl 
     transformation

-->

<xsl:template match="/">
  <xsl:apply-templates/>
</xsl:template>

<!-- bibitemMark Cleaning -->
<xsl:template match="thebibliography/bibitem/bibitemMark">
  <xsl:copy>
    <xsl:apply-templates select="@*"/>
    <xsl:apply-templates select="itemMark/text()|itemMark/*"/>
  </xsl:copy>
</xsl:template>

<!-- Node erasing: -->
<xsl:template match="verse/item"/>
<!-- need for tabular material <xsl:template match="rule"/> -->
<xsl:template match="verse/itemMark"/>
<xsl:template match="bibitemMark/item"/>

<!-- superscript, used in footnote mark, should contains only text,
     but often contents math tag, notably in \thanks...
-->
<xsl:template match="superscript">
  <xsl:copy>
    <xsl:value-of select="."/>
  </xsl:copy>
</xsl:template>

<!-- Empty paragraph cleaning 
     * transform multiple parsep to single a single one.
     * drop parsep juste after a section title
-->

<xsl:template match="parsep">
  <xsl:choose>
    <xsl:when test="name(following-sibling::*[position()=1])='parsep'">
    </xsl:when>
    <xsl:when test="name(preceding-sibling::*[position()=1])='sectionTitle'"/>
    <xsl:when test="name(preceding-sibling::*[position()=1])='section'"/>
    <xsl:when test="name(preceding-sibling::*[position()=1])='label' and name(preceding-sibling::*[position()=2])='section'"/>
    <xsl:otherwise>
      <xsl:copy-of select="."/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<!-- verbatim environment cleaning
     * transform verbatimSpace to nonbreaking-space entity &#xa0;
     * transform verbatimLineBreak to linebreak element <br>
     * As verbatim should not contains other tags, take the value-of the rest
-->
<xsl:template match="verb/verbatimSpace">
  <xsl:text>&#xa0;</xsl:text>
</xsl:template>
<xsl:template match="verbatim/verbatimSpace">
  <xsl:text>&#xa0;</xsl:text>
</xsl:template>
<xsl:template match="verbatim/verbatimLineBreak">
  <br/>
</xsl:template>
<xsl:template match="verbatim/*">
  <xsl:value-of select="."/>
</xsl:template>

<!-- Follow recursively the tree: -->
<xsl:template match="*|@*">
  <xsl:copy>
    <xsl:apply-templates select="*|@*|text()"/>
  </xsl:copy>
</xsl:template>

     

</xsl:stylesheet>
