<!--
%% footnote2xhtml.xsl
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
% This work consists of the files footnote2xhtml.xsl.
-->
<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
		xmlns="http://www.w3.org/1999/xhtml" 
		xmlns:lxir="http://www.latex-lxir.org">

  <xsl:template match="footnote|mpfootnote">
    <div class="{name()}">
      <xsl:apply-templates select="@*"/>
      <xsl:apply-templates/>
    </div>    
  </xsl:template>

  <xsl:template match="footnoteCall/footnoteMark">
    <span class="{name()}">
      <xsl:apply-templates select="@*"/>
      <a name="call_{@lxir:idFootnote}"/>
      <a class="{name()}" href="#{@lxir:idFootnote}">
	<xsl:apply-templates/>
      </a>
    </span>
  </xsl:template>

  <xsl:template match="footnote/footnoteMark">
    <span class="{name()}">
      <xsl:apply-templates select="@*"/>
      <a name="{@lxir:idFootnote}"/>
      <a class="{name()}" href="#call_{@lxir:idFootnote}">
	<xsl:apply-templates/>
      </a>
    </span>
  </xsl:template>



  <xsl:template match="footnotelist">
    <div class="{name()}">
      <xsl:apply-templates/>
    </div>
  </xsl:template>

  <xsl:template match="superscript|footnoteText|footnoteCall">
    <span class="{name()}">
      <xsl:apply-templates/>
    </span>    
  </xsl:template>

</xsl:stylesheet>