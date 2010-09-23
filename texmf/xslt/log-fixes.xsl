<?xml version="1.0" encoding="UTF-8"?>
<!--
%% log-fixes.xsl
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
% This work consists of the files log-fixes.xsl.
-->
<xsl:stylesheet version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>

<xsl:include href="log-prescript.xsl"/>
<xsl:include href="log-enlargesub.xsl"/>
<xsl:include href="log-negative.xsl"/>

<xsl:strip-space elements="*"/>
<xsl:output method="xml" indent="no" encoding="UTF-8"/>

<xsl:template match="@width|@height|@depth" />

<xsl:template match="*|@*">
	<xsl:copy>
		<xsl:apply-templates select="*|@*|text()"/>
	</xsl:copy>
</xsl:template>

</xsl:stylesheet>
