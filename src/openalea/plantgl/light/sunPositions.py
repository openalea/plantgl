# -*- python -*-
#
#       VegeSTAR 4
#
#       Copyright or (C) or Copr. 2007 INRA UMR547 PIAF
#
#       File author(s): Boris Adam <adam@clermont.inra.fr>
#                       Nicolas Dones <dones@clermont.inra.fr>
#
#       PIAF WebSite : http://www.clermont.inra.fr/piaf
#


import math

class Sequence:
    def __init__(self, parent=None):
        self.nbHauteurJour = 0
        self.nbHauteurMax = 0
        self.Duree = 0

    def getNbHauteurJour(self):
        return self.nbHauteurJour

    def declinaison(self, jour) :#As Integer) As Double
        #declinaison en rad
        #declinaison = (23.45 * Sin(2 * PI * (jour + 284) / 366)) * PI / 180
        SIDEC = 0.0
        TETA = 0.0
        OM = 0.0

        OM = 0.017202 * (jour - 3.244)
        TETA = OM + 0.03344 * math.sin(OM) * (1 + 0.021 * math.cos(OM)) - 1.3526
        SIDEC = 0.3978 * math.sin(TETA)
        return math.asin(SIDEC)

    def angleHoraireMax(self, declinaison, latitude): #declinaison As Double, latitude As Double) As Double
        return math.acos(-math.tan(declinaison) * math.tan(latitude))

    def azimut(self, latitude, declinaison, angleHoraire, hauteur): #latitude As Double, declinaison As Double, AngleHoraire As Double, hauteur As Double) As Double
        sinAz = 0.0
        cosAz = 0.0
        sinAz = math.cos(declinaison) * math.sin(angleHoraire) / math.cos(hauteur)
        cosAz = (math.sin(latitude) * math.cos(declinaison) * math.cos(angleHoraire) - math.cos(latitude) * math.sin(declinaison)) / math.cos(hauteur)
        if (cosAz >= 0) :
            return math.asin(sinAz)
        elif (sinAz >= 0) :
            return (math.pi - math.asin(sinAz))
        else:
            return (-math.pi - math.asin(sinAz))

    def angleHoraire(self, m): # m As Integer) As Single
        return (m * 0.25 - 180) * math.pi / 180.

    def decalLONG(self, longitude): #longitude As Double) As Double
        #decalage heure vraie
        if longitude < 0 :
            return (-(longitude % 15) * 4)
        else:
            return ((longitude % 15) * 4)

    def modVBA(self, n, d): # n As Double, d As Double) As Double
        return (n - d * int(round(n / d)))


    def heureTSV(self, jour, heure, decalSoleil, decalGMT, longitude): #jour As Integer, heure As Double) As Integer
        #retourne des minutes
        """ Compute solar time including time equation correction.
            With decalSoleil=0 and decalGMT=0, hour correspond to Universal Time Coordinated """
        EQTPS = 0.0
        DPHI = 0.0
        DPH1 = 0.0
        TPHI = 0.0
        TETA = 0.0
        OM = 0.0
        timeZone = 0

        OM = 0.017202 * (jour - 3.244)
        TETA = OM + 0.03344 * math.sin(OM) * (1 + 0.021 * math.cos(OM)) - 1.3526
        TPHI = 0.91747 * math.sin(TETA) / math.cos(TETA)
        DPH1 = math.atan(TPHI) - OM + 1.3526
        if DPH1 > -1 :
            DPHI = DPH1
        else:
            DPHI = self.modVBA(DPH1 + 1, math.pi) - 1
        EQTPS = -DPHI * 229.2
        #'TimeZone = -val(frmPosition.CbHORRAIRE.Text)
        timeZone = -decalSoleil - decalGMT
        return round((heure + timeZone + longitude / 15. + EQTPS / 60.) * 60)


    def relativeIntensity(self, jour, htsv, latitude):
        declinaison = self.declinaison(jour)
        anghoraire = (2 * math.pi) / (24 * (htsv - 12)) 
        cosAz = (math.sin(latitude) * math.sin(declinaison) + math.cos(latitude) * math.cos(declinaison)) * math.cos(anghoraire)
        return cosAz

    def hourly_diffuse_global_ratio(self, globalirradiance, jour, heureUTC, latitude, longitude):
        htsv = self.heureTSV(jour, heureUTC, 0, 0, longitude)
        relintensity = self.relativeIntensity(jour, htsv, latitude)
        I0 = 1370 * (1 + 0.033 *math.cos(2* math.pi * (jour -4) / 366))
        S0 = I0 * relintensity
        RsR0 = globalirradiance / S0 # ratio between irradiance of the sun at the top of atmosphere and irradiance receive at the soil.
        R = 0.847 - 1.61 * relintensity + 1.04 * (relintensity ** 2)
        K = (1.47 - R) / 1.66

        if RsR0 <= 0.22    : return 1
        elif RsR0 <=  0.35 : return (1- 6.4 * (RsR0 - 0.22) ** 2)
        elif RsR0 <= K     : return 1.47 - 1.66 * RsR0
        else: return R


    def positionSoleil(self, pasMinute, latitude, jour, mindeb, minfin) :
        # pasMinute : pas entre 2 positions du soleil en minutes
        # jour (julien in [1, 365])
        # mindeb, minfin : heure de debut et de fin en minute (heure TSV temps solaire vrai)

        # retourne un triplet (tabAZ, tabH, tabHeur)

        pasAngulaire = int(pasMinute) * 0.25 * math.pi / 180
        declin = 0.0
        declin = self.declinaison(jour)
        tabH = []
        tabAZ = []
        tabHeur = []

        demiDureeJour = (self.angleHoraireMax(declin, latitude) * 180 / math.pi) / 360. * 24
        coucher = int(round((12 + demiDureeJour) * 60))
        lever = int(round((12 - demiDureeJour) * 60))

        self.nbHauteurJour = int(round(((demiDureeJour * 2) / (pasMinute / 60.)) + 1))

        s = 0.
        indice = -1
        s = self.angleHoraire(mindeb)
        while s < self.angleHoraire(minfin + 1):
            if (self.angleHoraire(lever) <= s and s <= self.angleHoraire(coucher)) :
                indice += 1
                tabH.append( math.asin(math.sin(latitude) * math.sin(declin) + math.cos(latitude) * math.cos(declin) * math.cos(s)))
                tabAZ.append( self.azimut(latitude, declin, s, tabH[indice]))
                tabHeur.append( round(((s * 180 / math.pi) + 180) / 0.25) )
            s = s +pasAngulaire

        self.nbHauteurJour = indice+1

        if self.nbHauteurJour > self.nbHauteurMax :
            self.nbHauteurMax = self.nbHauteurJour


        return tabAZ, tabH, tabHeur


if __name__ == "__main__":
    seq = Sequence()
    jour = 172
    latitude = 45.78 #en d
    longitude = 3.08
    heureDeb = 0.5
    heureFin = 23.5
    pas = 30 
    hdeb = seq.heureTSV(jour, heureDeb, 1, 0, longitude)
    hfin = seq.heureTSV(jour, heureFin, 1, 0, longitude)
    print("Heure debut, fin : ",hdeb, hfin)
    declinaison = seq.declinaison(jour)
    print("Declinaison : ",declinaison)
    angleHoraireMax = seq.angleHoraireMax(declinaison,latitude*math.pi/180)
    print("Angle Horaire Max : ",angleHoraireMax)
    print("Angle horaire de 1362 : ",seq.angleHoraire(1362))
    (az,el,heur) = seq.positionSoleil(pas, latitude*math.pi/180, jour, 0, hfin)
    assert( len(az) == len(el) )
    for i in range(len(az)) :
        print("Azimuth : ", az[i] * 180. / math.pi,"  Elevation : ", el[i] * 180. / math.pi)
        
