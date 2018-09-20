using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UnitTests
{
    //[global::ProtoBuf.ProtoContract(Name = @"Tipo_Frecuencia")]
    public enum Tipo_Frecuencia
    {

        //[global::ProtoBuf.ProtoEnum(Name = @"Basica", Value = 0)]
        Basica = 0,

        //[global::ProtoBuf.ProtoEnum(Name = @"HF", Value = 1)]
        HF = 1,

        //[global::ProtoBuf.ProtoEnum(Name = @"VHF", Value = 2)]
        VHF = 2,

        //[global::ProtoBuf.ProtoEnum(Name = @"UHF", Value = 3)]
        UHF = 3,

        //[global::ProtoBuf.ProtoEnum(Name = @"DUAL", Value = 4)]
        DUAL = 4,

        //[global::ProtoBuf.ProtoEnum(Name = @"FD", Value = 5)]
        FD = 5,

        //[global::ProtoBuf.ProtoEnum(Name = @"ME", Value = 6)]
        ME = 6
    }

    //[global::ProtoBuf.ProtoContract(Name = @"Tipo_Canal")]
    public enum Tipo_Canal
    {

        //[global::ProtoBuf.ProtoEnum(Name = @"Monocanal", Value = 0)]
        Monocanal = 0,

        //[global::ProtoBuf.ProtoEnum(Name = @"Multicanal", Value = 1)]
        Multicanal = 1,

        //[global::ProtoBuf.ProtoEnum(Name = @"Otros", Value = 2)]
        Otros = 2
    }

    //[global::ProtoBuf.ProtoContract(Name = @"Tipo_Formato_Trabajo")]
    public enum Tipo_Formato_Trabajo
    {
        //[global::ProtoBuf.ProtoEnum(Name = @"Principal", Value = 0)]
        Principal = 0,

        //[global::ProtoBuf.ProtoEnum(Name = @"Reserva", Value = 1)]
        Reserva = 1,

        //[global::ProtoBuf.ProtoEnum(Name = @"Ambos", Value = 2)]
        Ambos = 2
    }

    //[global::ProtoBuf.ProtoContract(Name = @"Tipo_Formato_Frecuencia")]
    public enum Tipo_Formato_Frecuencia
    {
        //[global::ProtoBuf.ProtoEnum(Name = @"MHz", Value = 0)]
        MHz = 0,

        //[global::ProtoBuf.ProtoEnum(Name = @"Hz", Value = 1)]
        Hz = 1
    }

    //[global::ProtoBuf.ProtoContract(Name = @"GearChannelSpacings")]
    public enum GearChannelSpacings
    {
        //[global::ProtoBuf.ProtoEnum(Name = @"ChannelSpacingsDefault", Value = 0)]
        ChannelSpacingsDefault = 0,

        //[global::ProtoBuf.ProtoEnum(Name = @"kHz_8_33", Value = 1)]
        kHz_8_33 = 1,

        //[global::ProtoBuf.ProtoEnum(Name = @"kHz_12_5", Value = 2)]
        kHz_12_5 = 2,

        //[global::ProtoBuf.ProtoEnum(Name = @"kHz_25_00", Value = 3)]
        kHz_25_00 = 3
    }

    //[global::ProtoBuf.ProtoContract(Name = @"GearCarrierOffStatus")]
    public enum GearCarrierOffStatus
    {
        //[global::ProtoBuf.ProtoEnum(Name = @"Off", Value = 0)]
        Off = 0,

        //[global::ProtoBuf.ProtoEnum(Name = @"kHz_7_5", Value = 1)]
        kHz_7_5 = 1,

        //[global::ProtoBuf.ProtoEnum(Name = @"kHz_5_0", Value = 2)]
        kHz_5_0 = 2,

        //[global::ProtoBuf.ProtoEnum(Name = @"kHz_2_5", Value = 3)]
        kHz_2_5 = 3,

        //[global::ProtoBuf.ProtoEnum(Name = @"Hz_0_0", Value = 4)]
        Hz_0_0 = 4,

        //[global::ProtoBuf.ProtoEnum(Name = @"kHz_minus_2_5", Value = 5)]
        kHz_minus_2_5 = 5,

        //[global::ProtoBuf.ProtoEnum(Name = @"kHz_minus_5_0", Value = 6)]
        kHz_minus_5_0 = 6,

        //[global::ProtoBuf.ProtoEnum(Name = @"kHz_minus_7_5", Value = 7)]
        kHz_minus_7_5 = 7,

        //[global::ProtoBuf.ProtoEnum(Name = @"kHz_8", Value = 8)]
        kHz_8 = 8,

        //[global::ProtoBuf.ProtoEnum(Name = @"kHz_4", Value = 9)]
        kHz_4 = 9,

        //[global::ProtoBuf.ProtoEnum(Name = @"kHz_minus_4", Value = 10)]
        kHz_minus_4 = 10,

        //[global::ProtoBuf.ProtoEnum(Name = @"kHz_minus_8", Value = 11)]
        kHz_minus_8 = 11,

        //[global::ProtoBuf.ProtoEnum(Name = @"kHz_7_3", Value = 12)]
        kHz_7_3 = 12,

        //[global::ProtoBuf.ProtoEnum(Name = @"kHz_minus_7_3", Value = 13)]
        kHz_minus_7_3 = 13
    }

    //[global::ProtoBuf.ProtoContract(Name = @"GearModulations")]
    public enum GearModulations
    {

        //[global::ProtoBuf.ProtoEnum(Name = @"AM", Value = 0)]
        AM = 0,

        //[global::ProtoBuf.ProtoEnum(Name = @"Reserved", Value = 1)]
        Reserved = 1,

        //[global::ProtoBuf.ProtoEnum(Name = @"ACARS", Value = 2)]
        ACARS = 2,

        //[global::ProtoBuf.ProtoEnum(Name = @"VDL2", Value = 3)]
        VDL2 = 3,

        //[global::ProtoBuf.ProtoEnum(Name = @"AM_CT", Value = 4)]
        AM_CT = 4
    }

    //[global::ProtoBuf.ProtoContract(Name = @"GearPowerLevels")]
    public enum GearPowerLevels
    {

        //[global::ProtoBuf.ProtoEnum(Name = @"PowerLevelsDefault", Value = 0)]
        PowerLevelsDefault = 0,

        //[global::ProtoBuf.ProtoEnum(Name = @"Low", Value = 1)]
        Low = 1,

        //[global::ProtoBuf.ProtoEnum(Name = @"Normal", Value = 2)]
        Normal = 2
    }

    [global::System.Serializable/*, global::ProtoBuf.ProtoContract(Name = @"HfRangoFrecuencias")*/]
    public partial class HfRangoFrecuencias // : global::ProtoBuf.IExtensible
    {
        public HfRangoFrecuencias() { }

        private uint _fmin;
        //[global::ProtoBuf.ProtoMember(1, IsRequired = true, Name = @"fmin", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
        public uint fmin
        {
            get { return _fmin; }
            set { _fmin = value; }
        }
        private uint _fmax;
        //[global::ProtoBuf.ProtoMember(2, IsRequired = true, Name = @"fmax", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
        public uint fmax
        {
            get { return _fmax; }
            set { _fmax = value; }
        }
        //private global::ProtoBuf.IExtension extensionObject;
        //global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
        //{ return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
    }

    [global::System.Serializable/*, global::ProtoBuf.ProtoContract(Name = @"Node")*/]
    public partial class Node /*: global::ProtoBuf.IExtensible*/
    {
        public Node() { }

        private string _Id;
        //[global::ProtoBuf.ProtoMember(1, IsRequired = true, Name = @"Id", DataFormat = global::ProtoBuf.DataFormat.Default)]
        public string Id
        {
            get { return _Id; }
            set { _Id = value; }
        }
        private string _SipUri;
        //[global::ProtoBuf.ProtoMember(2, IsRequired = true, Name = @"SipUri", DataFormat = global::ProtoBuf.DataFormat.Default)]
        public string SipUri
        {
            get { return _SipUri; }
            set { _SipUri = value; }
        }
        private string _IpGestor;
        //[global::ProtoBuf.ProtoMember(3, IsRequired = true, Name = @"IpGestor", DataFormat = global::ProtoBuf.DataFormat.Default)]
        public string IpGestor
        {
            get { return _IpGestor; }
            set { _IpGestor = value; }
        }
        private string _Oid;
        //[global::ProtoBuf.ProtoMember(4, IsRequired = true, Name = @"Oid", DataFormat = global::ProtoBuf.DataFormat.Default)]
        public string Oid
        {
            get { return _Oid; }
            set { _Oid = value; }
        }
        private readonly global::System.Collections.Generic.List</*U5ki.Infrastructure.*/HfRangoFrecuencias> _Frecs = new global::System.Collections.Generic.List</*U5ki.Infrastructure.*/HfRangoFrecuencias>();
        //[global::ProtoBuf.ProtoMember(5, Name = @"Frecs", DataFormat = global::ProtoBuf.DataFormat.Default)]
        public global::System.Collections.Generic.List</*U5ki.Infrastructure.*/HfRangoFrecuencias> Frecs
        {
            get { return _Frecs; }
        }

        private bool _EsReceptor;
        //[global::ProtoBuf.ProtoMember(6, IsRequired = true, Name = @"EsReceptor", DataFormat = global::ProtoBuf.DataFormat.Default)]
        public bool EsReceptor
        {
            get { return _EsReceptor; }
            set { _EsReceptor = value; }
        }
        private bool _EsTransmisor;
        //[global::ProtoBuf.ProtoMember(7, IsRequired = true, Name = @"EsTransmisor", DataFormat = global::ProtoBuf.DataFormat.Default)]
        public bool EsTransmisor
        {
            get { return _EsTransmisor; }
            set { _EsTransmisor = value; }
        }
        private /*U5ki.Infrastructure.*/Tipo_Frecuencia _TipoDeFrecuencia;
        //[global::ProtoBuf.ProtoMember(8, IsRequired = true, Name = @"TipoDeFrecuencia", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
        public /*U5ki.Infrastructure.*/Tipo_Frecuencia TipoDeFrecuencia
        {
            get { return _TipoDeFrecuencia; }
            set { _TipoDeFrecuencia = value; }
        }
        private string _FrecuenciaPrincipal;
        //[global::ProtoBuf.ProtoMember(9, IsRequired = true, Name = @"FrecuenciaPrincipal", DataFormat = global::ProtoBuf.DataFormat.Default)]
        public string FrecuenciaPrincipal
        {
            get { return _FrecuenciaPrincipal; }
            set { _FrecuenciaPrincipal = value; }
        }
        private /*U5ki.Infrastructure.*/Tipo_Canal _TipoDeCanal;
        //[global::ProtoBuf.ProtoMember(10, IsRequired = true, Name = @"TipoDeCanal", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
        public /*U5ki.Infrastructure.*/Tipo_Canal TipoDeCanal
        {
            get { return _TipoDeCanal; }
            set { _TipoDeCanal = value; }
        }
        private /*U5ki.Infrastructure.*/Tipo_Formato_Trabajo _FormaDeTrabajo;
        //[global::ProtoBuf.ProtoMember(11, IsRequired = true, Name = @"FormaDeTrabajo", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
        public /*U5ki.Infrastructure.*/Tipo_Formato_Trabajo FormaDeTrabajo
        {
            get { return _FormaDeTrabajo; }
            set { _FormaDeTrabajo = value; }
        }
        private int _Prioridad;
        //[global::ProtoBuf.ProtoMember(12, IsRequired = true, Name = @"Prioridad", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
        public int Prioridad
        {
            get { return _Prioridad; }
            set { _Prioridad = value; }
        }

        private uint _Puerto = default(uint);
        //[global::ProtoBuf.ProtoMember(13, IsRequired = false, Name = @"Puerto", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
        [global::System.ComponentModel.DefaultValue(default(uint))]
        public uint Puerto
        {
            get { return _Puerto; }
            set { _Puerto = value; }
        }

        private /*U5ki.Infrastructure.*/GearCarrierOffStatus _Offset = /*U5ki.Infrastructure.*/GearCarrierOffStatus.Off;
        //[global::ProtoBuf.ProtoMember(14, IsRequired = false, Name = @"Offset", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
        [global::System.ComponentModel.DefaultValue(/*U5ki.Infrastructure.*/GearCarrierOffStatus.Off)]
        public /*U5ki.Infrastructure.*/GearCarrierOffStatus Offset
        {
            get { return _Offset; }
            set { _Offset = value; }
        }

        private /*U5ki.Infrastructure.*/GearChannelSpacings _Canalizacion = /*U5ki.Infrastructure.*/GearChannelSpacings.ChannelSpacingsDefault;
        //[global::ProtoBuf.ProtoMember(15, IsRequired = false, Name = @"Canalizacion", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
        [global::System.ComponentModel.DefaultValue(/*U5ki.Infrastructure.*/GearChannelSpacings.ChannelSpacingsDefault)]
        public /*U5ki.Infrastructure.*/GearChannelSpacings Canalizacion
        {
            get { return _Canalizacion; }
            set { _Canalizacion = value; }
        }

        private /*U5ki.Infrastructure.*/GearModulations _Modulacion = /*U5ki.Infrastructure.*/GearModulations.AM;
        //[global::ProtoBuf.ProtoMember(16, IsRequired = false, Name = @"Modulacion", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
        [global::System.ComponentModel.DefaultValue(/*U5ki.Infrastructure.*/GearModulations.AM)]
        public /*U5ki.Infrastructure.*/GearModulations Modulacion
        {
            get { return _Modulacion; }
            set { _Modulacion = value; }
        }

        private int _Potencia = default(int);
        //[global::ProtoBuf.ProtoMember(17, IsRequired = false, Name = @"Potencia", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
        [global::System.ComponentModel.DefaultValue(default(int))]
        public int Potencia
        {
            get { return _Potencia; }
            set { _Potencia = value; }
        }

        private /*U5ki.Infrastructure.*/Tipo_Formato_Frecuencia _FormatoFrecuenciaPrincipal = /*U5ki.Infrastructure.*/Tipo_Formato_Frecuencia.MHz;
        //[global::ProtoBuf.ProtoMember(18, IsRequired = false, Name = @"FormatoFrecuenciaPrincipal", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
        [global::System.ComponentModel.DefaultValue(/*U5ki.Infrastructure.*/Tipo_Formato_Frecuencia.MHz)]
        public /*U5ki.Infrastructure.*/Tipo_Formato_Frecuencia FormatoFrecuenciaPrincipal
        {
            get { return _FormatoFrecuenciaPrincipal; }
            set { _FormatoFrecuenciaPrincipal = value; }
        }

        private /*U5ki.Infrastructure.*/GearPowerLevels _NivelDePotencia = /*U5ki.Infrastructure.*/GearPowerLevels.PowerLevelsDefault;
        //[global::ProtoBuf.ProtoMember(19, IsRequired = false, Name = @"NivelDePotencia", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
        [global::System.ComponentModel.DefaultValue(/*U5ki.Infrastructure.*/GearPowerLevels.PowerLevelsDefault)]
        public /*U5ki.Infrastructure.*/GearPowerLevels NivelDePotencia
        {
            get { return _NivelDePotencia; }
            set { _NivelDePotencia = value; }
        }
        private string _FrecuenciaClave;
        //[global::ProtoBuf.ProtoMember(20, IsRequired = true, Name = @"FrecuenciaClave", DataFormat = global::ProtoBuf.DataFormat.Default)]
        public string FrecuenciaClave
        {
            get { return _FrecuenciaClave; }
            set { _FrecuenciaClave = value; }
        }
        private int _ModeloEquipo;
        //[global::ProtoBuf.ProtoMember(21, IsRequired = true, Name = @"ModeloEquipo", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
        public int ModeloEquipo
        {
            get { return _ModeloEquipo; }
            set { _ModeloEquipo = value; }
        }

        private string _idEmplazamiento = "";
        //[global::ProtoBuf.ProtoMember(22, IsRequired = false, Name = @"idEmplazamiento", DataFormat = global::ProtoBuf.DataFormat.Default)]
        [global::System.ComponentModel.DefaultValue("")]
        public string idEmplazamiento
        {
            get { return _idEmplazamiento; }
            set { _idEmplazamiento = value; }
        }
        //private global::ProtoBuf.IExtension extensionObject;
        //global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
        //{ return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
    }

}
