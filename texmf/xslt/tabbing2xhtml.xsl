<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
		xmlns="http://www.w3.org/1999/xhtml" >
  <xsl:template match="tabbing">
    <div class="{name()}">
      <table class="{name()}">
	<xsl:apply-templates select="tabLine"/>
      </table>
    </div>
  </xsl:template>

  <xsl:template match="tabLine">
    <tr class="{name()}">
      <xsl:apply-templates select="tabField"/>
    </tr>
  </xsl:template>


  <xsl:template match="tabField">
      <td class="{name()}">
      <xsl:apply-templates/>
    </td>
  </xsl:template>

</xsl:stylesheet>