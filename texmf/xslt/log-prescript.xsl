<?xml version="1.0" encoding="UTF-8"?>
<!--
%% log-prescript.xsl
%% part of lxir - a tool to convert latex documents to xml.
%% Copyright 2010 EDP-Sciences
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
% The Current Maintainer of this work is Xavier Trochu.
%
% This work consists of the files log-prescript.xsl.
-->
<xsl:stylesheet version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>

<xsl:output method="xml" indent="no" encoding="UTF-8"/>

<!--
	This template replace the following construct:
	<msup single="1">
		<foo />
	</msup>
	<msub>
		<bar1 />
		<bar2 />
	</msub>

	into :

	<mmultiscripts>
		<bar1/>
		<none/>
		<bar2/>
		<mprescripts />
		<none/>
		<foo/>
	</mmultiscripts>

	all similar cases are treated
-->

<xsl:template match="msup[@single='1']">
	<xsl:choose>
		<xsl:when test="following-sibling::*[position() = 1][self::msub]">
			<mmultiscripts>
				<xsl:apply-templates select="following-sibling::*[position() = 1]/child::*[position() = 1]" />
				<xsl:apply-templates select="following-sibling::*[position() = 1]/child::*[position() = last()]" />
				<none />
				<mprescripts />
				<none />
				<xsl:apply-templates select="*" />
			</mmultiscripts>
		</xsl:when>
		<xsl:when test="following-sibling::*[position() = 1][self::msup]">
			<mmultiscripts>
				<xsl:apply-templates select="following-sibling::*[position() = 1]/child::*[position() = 1]" />
				<none />
				<xsl:apply-templates select="following-sibling::*[position() = 1]/child::*[position() = last()]" />
				<mprescripts />
				<none />
				<xsl:apply-templates select="*" />
			</mmultiscripts>
		</xsl:when>
		<xsl:when test="following-sibling::*[position() = 1][self::msubsup]">
			<mmultiscripts>
				<xsl:apply-templates select="following-sibling::*[position() = 1]/*" />
				<mprescripts />
				<none />
				<xsl:apply-templates select="*" />
			</mmultiscripts>
		</xsl:when>
		<xsl:otherwise>
			<xsl:copy>
				<xsl:apply-templates select="*|@*|text()"/>
			</xsl:copy>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template match="msub[@single='1']">
	<xsl:choose>
		<xsl:when test="following-sibling::*[position() = 1][self::msub]">
			<mmultiscripts>
				<xsl:apply-templates select="following-sibling::*[position() = 1]/child::*[position() = 1]" />
				<xsl:apply-templates select="following-sibling::*[position() = 1]/child::*[position() = last()]" />
				<none />
				<mprescripts />
				<xsl:apply-templates select="*" />
				<none />
			</mmultiscripts>
		</xsl:when>
		<xsl:when test="following-sibling::*[position() = 1][self::msup]">
			<mmultiscripts>
				<xsl:apply-templates select="following-sibling::*[position() = 1]/child::*[position() = 1]" />
				<none />
				<xsl:apply-templates select="following-sibling::*[position() = 1]/child::*[position() = last()]" />
				<mprescripts />
				<xsl:apply-templates select="*" />
				<none />
			</mmultiscripts>
		</xsl:when>
		<xsl:when test="following-sibling::*[position() = 1][self::msubsup]">
			<mmultiscripts>
				<xsl:apply-templates select="following-sibling::*[position() = 1]/*" />
				<mprescripts />
				<xsl:apply-templates select="*" />
				<none />
			</mmultiscripts>
		</xsl:when>
		<xsl:otherwise>
			<xsl:copy>
				<xsl:apply-templates select="*|@*|text()"/>
			</xsl:copy>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template match="*[self::msup or self::msub or self::msubsup][preceding-sibling::*[position() = 1][self::msup or self::msub][@single='1']]" />

</xsl:stylesheet>
