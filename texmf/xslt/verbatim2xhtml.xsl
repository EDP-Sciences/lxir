<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
		xmlns="http://www.w3.org/1999/xhtml"
		xmlns:mm="http://www.w3.org/1998/Math/MathML">

<xsl:template match="verbatim">
	<div class="{name()}">
		<xsl:apply-templates select="@*"/>
		<xsl:apply-templates mode="in-verbatim" select="br|text()"/>
	</div>
</xsl:template>

<xsl:template match="br" mode="in-verbatim">
	<xsl:text>
</xsl:text>
</xsl:template>

<xsl:template match="text()" mode="in-verbatim">
	<xsl:copy />
</xsl:template>

</xsl:stylesheet>
