<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" 
		xmlns="http://www.w3.org/1999/xhtml"
		xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
		xmlns:lxir="http://www.latex-lxir.org"
  >
    <xsl:template match="text">
      <span class="{name()}">
	<xsl:copy-of select="@lxir:family"/>
	<xsl:copy-of select="@lxir:series"/>
	<xsl:copy-of select="@lxir:shape"/>
	<xsl:copy-of select="@lxir:size"/>	
	<xsl:value-of select="."/>
      </span>
    </xsl:template>
</xsl:stylesheet>