<!--
%% biblio2xhtml.xsl
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
% This work consists of the files biblio2xhtml.xsl.
-->
<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
		xmlns="http://www.w3.org/1999/xhtml" 
		xmlns:lxir="http://www.latex-lxir.org">

  <xsl:template match="thebibliography">
    <div class="{name()}">
      <hr/>
      <xsl:apply-templates select="thebibliographyMark"/>
      <hr/>
      <xsl:apply-templates select="text()|*[name() != 'thebibliographyMark']"/>
    </div>
    <!-- line break for better debug -->
    <xsl:text>&#xa;</xsl:text>
  </xsl:template>


  <xsl:template match="thebibliographyMark">
    <div class="{name()}">
      <xsl:apply-templates/>
    </div>
  </xsl:template>

<!--
<bibitem id="31" key="bmf94" close-id="37">
<bibitemMark id="32" close-id="36">[Bodo et al.(1994)Bodo, Massaglia, Ferrari, &amp; Trussoni]</bibitemMark>
Bodo, G., Massaglia, S., Ferrari, A., &amp; Trussoni, E. 1994, Another journal , 283, 655
</bibitem>
-->
  <xsl:template match="bibitem">
    <div  class="{name()}">
      <xsl:copy-of select="@*"/>
      <a name="{@lxir:key}"/>
      <xsl:apply-templates select="bibitemMark"/>
      <xsl:apply-templates select="text()|node()[name() != 'bibitemMark']"/>
    </div>
    <!-- line break for better debug -->
    <xsl:text>&#xa;</xsl:text>
  </xsl:template>

  <xsl:template match="bibitemMark">
    <span class="{name()}">
      <xsl:apply-templates/>
    </span>
  </xsl:template>

<!--
    <cite id="1288" pre-note="" post-note="" key="her50" close-id="1289">Herbig 1950</cite>
-->

<xsl:template match="cite">
   <a class="{name()}" href="#{@lxir:key}" key="{@lxir:key}">
    <xsl:apply-templates/>
    </a>
</xsl:template>

</xsl:stylesheet>