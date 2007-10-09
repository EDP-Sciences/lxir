<?xml version="1.0" encoding="utf-8"?>
<!--
%% list2xhtml.xsl
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
% This work consists of the files list2xhtml.xsl.
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
		xmlns="http://www.w3.org/1999/xhtml" 
		xmlns:lxir="http://www.latex-lxir.org"
		>

  <xsl:template match="list">
    <div class="{@lxir:listType}">
      <xsl:apply-templates/>
    </div>
    <!-- line break for better debug -->
    <xsl:text>&#xa;</xsl:text>
  </xsl:template>

  <xsl:template match="item">
    <div class="{name()}">
      <xsl:apply-templates/>
    </div>    
    <!-- line break for better debug -->
    <xsl:text>&#xa;</xsl:text>
  </xsl:template>

  <xsl:template match="itemMark">
    <span class="{name()}">
      <xsl:apply-templates/>
    </span>    
  </xsl:template>
</xsl:stylesheet>
