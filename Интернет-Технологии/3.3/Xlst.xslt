<?xml version="1.0" encoding="windows-1251"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

<xsl:template name="while">
	<xsl:param name="i"/>
	<xsl:param name="result"/>
	<xsl:choose>
		<xsl:when test="$i != 0">
			<xsl:call-template name="while">
				<xsl:with-param name="i" select="$i - 1"/>
				<xsl:with-param name="result" select="$result + 1"/>
			</xsl:call-template>
		</xsl:when>
		<xsl:otherwise>
			<xsl:value-of select="$result"/>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template match="/solar">
<xsl:variable name = "index" select = "0"/>
<title>Лабораторная работа 3.3</title>
<table width="80%" align="center" border="1" cellpadding="2">
	<th align="center" bgcolor="#ffc500">Индекс</th>
	<th align="center" bgcolor="#ffc500">Название</th>
	<th align="center" bgcolor="#ffc500">Расстояние от солнца(а.е)</th>
	<th align="center" bgcolor="#ffc500">Период обращения(г.)</th>
	<th align="center" bgcolor="#ffc500">Период вращения вокруг своей оси(ч.)</th>
	<th align="center" bgcolor="#ffc500">Масса относительно Земли</th>
	<th align="center" bgcolor="#ffc500">Диаметр(км.)</th>
	<th align="center" bgcolor="#ffc500">Количество спутников</th>
    
	<xsl:for-each select="planet[1] | planet[2] | planet[3] | planet[4] | planet[5] | planet[6] | planet[7] | planet[8] | planet[9]">
		<xsl:sort order="ascending" select="number(satellite_number)" data-type="number"/>
		<xsl:sort order="ascending" select="@caption"/>
			<tr>

				<xsl:choose>
					<xsl:when test="diametr &lt; 10000">
						<td align="center" bgcolor="#0050c4">
							<xsl:value-of select="$index + 1"/>
						</td>
						<td align="center" bgcolor="#0050c4">
							<xsl:value-of select="@caption"/>
						</td>
						<td align="center" bgcolor="#0050c4">
							<xsl:value-of select="distance"/>
						</td>
						<td align="center" bgcolor="#0050c4">
							<xsl:value-of select="circulation_period"/>
						</td>
						<td align="center" bgcolor="#0050c4">
							<xsl:value-of select="revolution_period"/>
						</td>
						<td align="center" bgcolor="#0050c4">
							<xsl:value-of select="weight"/>
						</td>
						<td align="center" bgcolor="#0050c4">
							<xsl:value-of select="diametr"/>
						</td>
						<td align="center" bgcolor="#0050c4">
							<xsl:value-of select="satellite_number"/>
						</td>
					</xsl:when>
					<xsl:otherwise>
						<td align="center" bgcolor="#007500">
							<xsl:value-of select="$index + 1"/>
						</td>
						<td align="center" bgcolor="#007500">
							<xsl:value-of select="@caption"/>
						</td>
						<td align="center" bgcolor="#007500">
							<xsl:value-of select="distance"/>
						</td>
						<td align="center" bgcolor="#007500">
							<xsl:value-of select="circulation_period"/>
						</td>
						<td align="center" bgcolor="#007500">
							<xsl:value-of select="revolution_period"/>
						</td>
						<td align="center" bgcolor="#007500">
							<xsl:value-of select="weight"/>
						</td>
						<td align="center" bgcolor="#007500">
							<xsl:value-of select="diametr"/>
						</td>
						<td align="center" bgcolor="#007500">
							<xsl:value-of select="satellite_number"/>
						</td>
					</xsl:otherwise>
				</xsl:choose>
			</tr>
		</xsl:for-each>
	</table>

</xsl:template>
</xsl:stylesheet>
