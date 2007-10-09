<?xml version="1.0" encoding="utf-8"?>
<!--
%% footnote.xsl
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
% This work consists of the files footnote.xsl.
-->
<xsl:stylesheet version="1.0" 
		xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
		xmlns:lxir="http://www.latex-lxir.org">
<xsl:output method="xml" 
	    indent="yes" 
	    />

<!-- footnote cleaning: we rebuild a xlatex call for footnote   -->
<!-- original structure for footnotecall/footnote is quit comple-->
<!-- and depends on the context :				-->
<!-- 								-->
<!-- * Outside minipage:					-->
<!-- ===================					-->
<!-- 								-->
<!-- CASE (1)							-->
<!-- \footnode[4]{a footnote} leads to:				-->
<!-- 								-->
<!--     <footnoteCall id="127" page="1" deffnmark="5" close-id="142">        --> 
<!--       <footnoteMark id="129" page="1" close-id="132">		      -->
<!--         <superscript>5</superscript>				      -->
<!--       </footnoteMark>						      -->
<!--       <text font="14" h="4336238" v="19436458"> </text>		      -->
<!--       <footnoteText id="140" close-id="141">			      -->
<!--         <text font="14" h="4336238" v="19436458">a footnote</text>	      -->
<!--       </footnoteText>                    				      -->
<!--                                                                          --> 
<!-- 								-->
<!-- ....							-->
<!--    <footnote id="134" page="1" close-id="139">                          -->
<!--       <superscript>5</superscript>					     -->
<!--       <footnoteText id="137" close-id="138">			     -->
<!--         <text font="25" h="5065689" v="39516306">a footnote</text>	     -->
<!--       </footnoteText>						     -->
<!--     </footnote>							     -->
<!--                                                                         -->    
<!-- CASE (2)							-->
<!-- \footnode{a footnote} leads to:				-->
<!--     <footnoteCall id="127" page="1" close-id="142">                 -->  
<!--       <footnoteMark id="129" page="1" close-id="132">		 -->
<!--         <superscript>3</superscript>				 -->
<!--       </footnoteMark>						 -->
<!--       <text font="14" h="4336238" v="19436458"> </text>		 -->
<!--       <footnoteText id="140" close-id="141">			 -->
<!--         <text font="14" h="4336238" v="19436458">a footnote</text>	 -->
<!--       </footnoteText>						 -->
<!--     </footnoteCall>                                                 -->  
<!-- ....							-->
<!--    <footnote id="134" page="1" close-id="139">                       -->   
<!--      <superscript>3</superscript>					  -->
<!--      <footnoteText id="137" close-id="138">			  -->
<!--        <text font="25" h="5065689" v="39516306">a footnote</text>	  -->
<!--      </footnoteText>						  -->
<!--    </footnote>							  -->
<!--                                                                      -->
<!-- 								-->
<!-- * Inside a minipage:					-->
<!-- ===================					-->
<!-- 								-->
<!-- CASE (4)							-->
<!-- \footnote{inside minipage} leads to:			-->
<!--     <footnoteCall id="75" page="1" close-id="91">                          -->                   
<!--        <footnoteMark id="77" page="1" close-id="81">			-->
<!--          <superscript>a</superscript>					-->
<!--        </footnoteMark>							-->
<!--        <text font="26" h="5453964" v="14407348"> </text>			-->
<!--        <footnoteText id="89" close-id="90">				-->
<!--          <text font="14" h="5453964" v="14407348">inside minipage</text>	-->
<!--        </footnoteText>							-->
<!--      </footnoteCall>                                                       -->    
<!-- ....							-->
<!--      <mpfootnote id="83" page="1" close-id="88">                           -->
<!--        <superscript>a</superscript>					-->
<!--        <footnoteText id="86" close-id="87">				-->
<!--          <text font="14" h="6225633" v="15575932">inside minipage</text>	-->
<!--        </footnoteText>							-->
<!--      </mpfootnote>                                                         -->								
<!-- CASE (5)							-->
<!-- \footnotemark{} inside minipage leads to			-->
<!--      <footnoteMark id="93" page="1" close-id="96">     -->
<!--        <superscript>2</superscript>		    -->
<!--      </footnoteMark>                                   -->
<!-- CASE (6)							-->
<!-- and the pertaining 					-->
<!--\footnotetext{This is a global footnote.}                   -->
<!-- located after the minipage:				-->
<!--    <footnote id="109" page="1" close-id="114">                                       -->
<!--       <superscript>2</superscript>							  -->
<!--       <footnoteText id="112" close-id="113">					  -->
<!--         <text font="25" h="5065689" v="38893713">This is a global footnote.</text>	  -->
<!--       </footnoteText>								  -->
<!--     </footnote>                                                                      -->
<!-- 								-->

<!-- OUTPUT: 
We aim to: 
- put all <footnote> but those of minipage a the end of the document in a  <footnotelist>
- build an @idFootnote for the footnote, containing the footnoteMark and the page
- add this @idFootnote in both  <footnoteCall> and <footnote>
- remove the <footnoteText> from <footnote>, as it is already in <footnote>
-->

<xsl:template match="/document/latex">
  <xsl:copy>
    <xsl:apply-templates select="*[name()!='footnote']"/>
    <xsl:if test="//footnote">
      <footnotelist>
	<xsl:apply-templates select="//footnote"/>
      </footnotelist>
    </xsl:if>
  </xsl:copy>
</xsl:template>

<xsl:template name="buildIdFootnote">
  <xsl:param name="page"/>
  <xsl:param name="mark"/>
  <xsl:text>fn_p.</xsl:text>
  <xsl:value-of select="normalize-space($page)"/>
  <xsl:text>_n.</xsl:text>
  <xsl:value-of select="normalize-space($mark)"/>
</xsl:template>

<!-- add an id containing the page + the footnote mark -->
<xsl:template match="footnoteMark">
 <xsl:copy>
    <xsl:attribute name="lxir:idFootnote">
      <xsl:call-template name="buildIdFootnote">
	<xsl:with-param name="page" select="@lxir:page"/>
	<xsl:with-param name="mark">
	  <xsl:value-of select="."/>
	</xsl:with-param>
      </xsl:call-template>
    </xsl:attribute>
    <xsl:apply-templates select="@*"/>
     <xsl:apply-templates/>
  </xsl:copy>
</xsl:template>

<!-- drop footnoteCall/footnoteText, as we always have it in footnote/footnoteText -->
<xsl:template match="footnoteCall/footnoteText"/>

<!-- add an id containing the page + the footnote mark -->
<xsl:template match="footnote">
  <xsl:copy>
    <xsl:attribute name="lxir:idFootnote">
      <xsl:call-template name="buildIdFootnote">
	<xsl:with-param name="page" select="@lxir:page"/>
	<xsl:with-param name="mark">
	  <xsl:value-of select="footnoteMark"/>
	</xsl:with-param>
      </xsl:call-template>
    </xsl:attribute>
    <xsl:apply-templates select="@*"/>
    <xsl:apply-templates/>
  </xsl:copy>
</xsl:template>

<xsl:template match="*|@*">
    <xsl:copy>
      <xsl:apply-templates select="*[name(.)!= 'footnote']|@*|text()"/>
    </xsl:copy>
</xsl:template>

</xsl:stylesheet>